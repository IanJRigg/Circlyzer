#include "gtest/gtest.h"
#include "circlyzer/network.h"
#include "circlyzer/component.h"
#include "circlyzer/units.h"

#include <memory>

/**********************************************************************************************//**
 * Assess that a Network is initialized with the node counter and element counter set to 0
 *************************************************************************************************/
TEST(Network, Constructor)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);
}

/**********************************************************************************************//**
 * Assess that nodes get created, UIDs get assigned as expected and the node counter increments
 * accordingly
 *************************************************************************************************/
TEST(Network, NodeCreation)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);

    auto first_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_elements(), 0);
    EXPECT_EQ(first_uid, 0);

    auto second_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 2);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 2);
    EXPECT_EQ(network.get_number_of_elements(), 0);
    EXPECT_EQ(second_uid, 1);
}

/**********************************************************************************************//**
 * Assess that elements get created, UIDs get assigned as expected and the element counter 
 * increments accordingly
 *************************************************************************************************/
TEST(Network, ElementCreation)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);

    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor));

    // Make sure that ownership has been correctly transfered away from resistor
    EXPECT_EQ(resistor.get(), nullptr);

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 1);
    EXPECT_EQ(first_uid, 0);
}

/**********************************************************************************************//**
 * Assess that accessing an invalid UID will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidUid)
{
    Network network;
    EXPECT_THROW(network.get_component(0xDEADBEEFU), std::out_of_range);
}

/**********************************************************************************************//**
 * Assess that accessing with a valid UID, but invalid type will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node();
    EXPECT_THROW(network.get_component(first_uid), std::runtime_error);
}

/**********************************************************************************************//**
 * Assess that a component can be created successfully, and that the same component can be accessed
 * using the provided UID
 *************************************************************************************************/
TEST(Network, AccessComponent)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor));
    const auto& component = network.get_component(first_uid);
    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, 1.0_ohm);
}

/**********************************************************************************************//**
 * Assess that an exception is thrown when using an invalid alias
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidAlias)
{
    Network network;
    EXPECT_THROW(network.get_component("DEAD BEEF!"), std::out_of_range);
}

/**********************************************************************************************//**
 * Assess that accessing with a valid alias, but invalid type will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentViaAliasWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node("DEAD BEEF!");
    EXPECT_THROW(network.get_component(first_uid), std::runtime_error);
}

/**********************************************************************************************//**
 * Assess that a component can be created successfully, and that the same component can be accessed
 * using the provided alias
 *************************************************************************************************/
TEST(Network, AccessComponentViaAlias)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor), "DEAD BEEF!");
    const auto& component = network.get_component("DEAD BEEF!");

    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, 1.0_ohm);
}

/**********************************************************************************************//**
 * Assess that destroying a node that doesn't exist changes nothing
 *************************************************************************************************/
TEST(Network, DestroyNodeWhenNoneExist)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);

    // Destroy a fake node
    network.destroy_node(0xDEADBEEF);

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);
}
