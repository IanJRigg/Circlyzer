#include "gtest/gtest.h"
#include "circlyzer/network.h"
#include "circlyzer/component.h"
#include "circlyzer/units.h"
#include "circlyzer/exceptions.h"

#include <memory>

using namespace Circlyzer;

namespace
{
    constexpr auto INVALID_UID = 0xDEADBEEFU;

    const auto DEFAULT_RESISTANCE = 1.0_ohm;

    const std::string VALID_ALIAS_ONE = "ONE";
    const std::string VALID_ALIAS_TWO = "TWO";
    const std::string TOO_LONG_ALIAS = "This is an alias that is absolutely longer than 25 chars";
}

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
 * Assess that an alias longer than the default length throws an exception
 *************************************************************************************************/
TEST(Network, NodeCreationWithTooLongAlias)
{
    Network network;
    EXPECT_THROW(network.create_node(TOO_LONG_ALIAS), Invalid_Alias_Exception);
}

/**********************************************************************************************//**
 * Assess that duplicate aliases throw an exception
 *************************************************************************************************/
TEST(Network, NodeCreationWithDuplicateAlias)
{
    Network network;
    network.create_node(VALID_ALIAS_ONE);
    EXPECT_THROW(network.create_node(VALID_ALIAS_ONE), Duplicate_Alias_Exception);
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
 * Assess that an alias longer than the default length throws an exception
 *************************************************************************************************/
TEST(Network, ElementCreationWithTooLongAlias)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    EXPECT_THROW(network.create_element(std::move(resistor), TOO_LONG_ALIAS), 
                 Invalid_Alias_Exception);
}

/**********************************************************************************************//**
 * Assess that duplicate aliases throw an exception
 *************************************************************************************************/
TEST(Network, ElementCreationWithDuplicateAlias)
{
    Network network;
    auto resistor_one = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    network.create_element(std::move(resistor_one), VALID_ALIAS_ONE);

    auto resistor_two = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    EXPECT_THROW(network.create_element(std::move(resistor_two), VALID_ALIAS_ONE),
                 Duplicate_Alias_Exception);
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

    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
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
    EXPECT_THROW(network.get_component(INVALID_UID), Non_Existant_UID_Exception);
}

/**********************************************************************************************//**
 * Assess that accessing with a valid UID, but invalid type will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node();
    EXPECT_THROW(network.get_component(first_uid), Wrong_Entity_Type_Exception);
}

/**********************************************************************************************//**
 * Assess that a component can be created successfully, and that the same component can be accessed
 * using the provided UID
 *************************************************************************************************/
TEST(Network, AccessComponent)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    auto first_uid = network.create_element(std::move(resistor));
    const auto& component = network.get_component(first_uid);
    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, DEFAULT_RESISTANCE);
}

/**********************************************************************************************//**
 * Assess that an exception is thrown when using an invalid alias
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidAlias)
{
    Network network;
    EXPECT_THROW(network.get_component(VALID_ALIAS_ONE), Non_Existant_Alias_Exception);
}

/**********************************************************************************************//**
 * Assess that accessing with a valid alias, but invalid type will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentViaAliasWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node(VALID_ALIAS_ONE);
    EXPECT_THROW(network.get_component(first_uid), Wrong_Entity_Type_Exception);
}

/**********************************************************************************************//**
 * Assess that a component can be created successfully, and that the same component can be accessed
 * using the provided alias
 *************************************************************************************************/
TEST(Network, AccessComponentViaAlias)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    auto first_uid = network.create_element(std::move(resistor), VALID_ALIAS_ONE);
    const auto& component = network.get_component(VALID_ALIAS_ONE);

    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, DEFAULT_RESISTANCE);
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
    network.destroy_node(INVALID_UID);

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);
}
