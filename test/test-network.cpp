#include "gtest/gtest.h"
#include "circlyzer/network.h"
#include "circlyzer/component.h"
#include "circlyzer/units.h"
#include "circlyzer/exceptions.h"

#include <memory>

using namespace Circlyzer;

namespace
{
    constexpr auto INVALID_UID_ONE = 0xDEADBEEFU;
    constexpr auto INVLAID_UID_TWO = 0xDEADBEEEU;

    const auto DEFAULT_RESISTANCE = 1.0_ohm;

    const std::string VALID_ALIAS_ONE = "ONE";
    const std::string VALID_ALIAS_TWO = "TWO";
    const std::string INVALID_ALIAS = "I'm an invalid alias!";
    const std::string TOO_LONG_ALIAS = "This is an alias that is absolutely longer than 25 chars";
}

/**********************************************************************************************//**
 * Assess that a Network is initialized with the node counter and branch counter set to 0
 *************************************************************************************************/
TEST(Network, Constructor)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
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
    EXPECT_EQ(network.get_number_of_branches(), 0);

    auto first_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_branches(), 0);
    EXPECT_EQ(first_uid, 0);

    auto second_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 2);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 2);
    EXPECT_EQ(network.get_number_of_branches(), 0);
    EXPECT_EQ(second_uid, 1);
}

/**********************************************************************************************//**
 * Assess that an alias longer than the default length throws an exception
 *************************************************************************************************/
TEST(Network, BranchCreationWithTooLongAlias)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    EXPECT_THROW(network.create_branch(std::move(resistor), TOO_LONG_ALIAS), 
                 Invalid_Alias_Exception);
}

/**********************************************************************************************//**
 * Assess that duplicate aliases throw an exception
 *************************************************************************************************/
TEST(Network, BranchCreationWithDuplicateAlias)
{
    Network network;
    auto resistor_one = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    network.create_branch(std::move(resistor_one), VALID_ALIAS_ONE);

    auto resistor_two = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    EXPECT_THROW(network.create_branch(std::move(resistor_two), VALID_ALIAS_ONE),
                 Duplicate_Alias_Exception);
}

/**********************************************************************************************//**
 * Assess that branches get created, UIDs get assigned as expected and the branch counter 
 * increments accordingly
 *************************************************************************************************/
TEST(Network, BranchCreation)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);

    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    auto first_uid = network.create_branch(std::move(resistor));

    // Make sure that ownership has been correctly transfered away from resistor
    EXPECT_EQ(resistor.get(), nullptr);

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 1);
    EXPECT_EQ(first_uid, 0);
}

/**********************************************************************************************//**
 * Assess that accessing an invalid UID will throw an exception
 *************************************************************************************************/
TEST(Network, AccessComponentWithInvalidUid)
{
    Network network;
    EXPECT_THROW(network.get_component(INVALID_UID_ONE), Non_Existant_UID_Exception);
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
    auto first_uid = network.create_branch(std::move(resistor));
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
    auto first_uid = network.create_branch(std::move(resistor), VALID_ALIAS_ONE);
    const auto& component = network.get_component(VALID_ALIAS_ONE);

    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, DEFAULT_RESISTANCE);
}

/**********************************************************************************************//**
 * 
 *************************************************************************************************/
TEST(Network, CreateConnectionWhenUIDsDontExist)
{
    Network network;

    network.create_connection_between(INVALID_UID_ONE, INVLAID_UID_TWO);

    auto first_uid = network.create_node(VALID_ALIAS_ONE);
}

/**********************************************************************************************//**
 * Assess that destroying a node that doesn't exist changes nothing
 *************************************************************************************************/
TEST(Network, DestroyEntityWhenNoneExist)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);

    // Destroy a fake node
    network.destroy_entity(INVALID_UID_ONE);

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}

/**********************************************************************************************//**
 * Assess that you destroying a node through destroy_entity works
 *************************************************************************************************/
TEST(Network, DestroyEntityToDestroyNode)
{
    Network network;
    auto node_uid = network.create_node();

    // Ensure that the counts are correct prrior to calling the deletion logic
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_branches(), 0);

    // Destroy the node. This should clear all the entity counts
    network.destroy_entity(node_uid);

    // Check that the entity counts remain unchanged
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}

/**********************************************************************************************//**
 * Assess that you destroying an branch through destroy_entity works
 *************************************************************************************************/
TEST(Network, DestroyEntityToDestroyBranch)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    auto branch_uid = network.create_branch(std::move(resistor));

    // Ensure that the counts are correct prrior to calling the deletion logic
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 1);

    // Destroy the branch. This should clear all the entity counts
    network.destroy_entity(branch_uid);

    // Check that the entity counts remain unchanged
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}

/**********************************************************************************************//**
 * Assess that destroying a node that doesn't exist changes nothing
 *************************************************************************************************/
TEST(Network, DestroyEntityViaAliasWhenNoneExist)
{
    Network network;
    auto node_uid = network.create_node(VALID_ALIAS_ONE);

    // Check entity counts
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 1);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_branches(), 0);

    // Destroy a fake node
    network.destroy_entity(INVALID_ALIAS);

    // Check that entity counts haven't changed
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 1);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}

/**********************************************************************************************//**
 * Assess that you destroying a node through destroy_entity works
 *************************************************************************************************/
TEST(Network, DestroyEntityViaAliasToDestroyNode)
{
    Network network;
    auto node_uid = network.create_node(VALID_ALIAS_ONE);

    // Ensure that the counts are correct prrior to calling the deletion logic
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 1);
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_branches(), 0);

    // Destroy the node. This should clear all the entity counts
    network.destroy_entity(VALID_ALIAS_ONE);

    // Check that the entity counts remain unchanged
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}

/**********************************************************************************************//**
 * Assess that you destroying an branch through destroy_entity works
 *************************************************************************************************/
TEST(Network, DestroyEntityViaAliasToDestroyBranch)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(DEFAULT_RESISTANCE);
    auto branch_uid = network.create_branch(std::move(resistor), VALID_ALIAS_ONE);

    // Ensure that the counts are correct prrior to calling the deletion logic
    EXPECT_EQ(network.get_number_of_entities(), 1);
    EXPECT_EQ(network.get_number_of_aliases(), 1);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 1);

    // Destroy the branch. This should clear all the entity counts
    network.destroy_entity(VALID_ALIAS_ONE);

    // Check that the entity counts remain unchanged
    EXPECT_EQ(network.get_number_of_entities(), 0);
    EXPECT_EQ(network.get_number_of_aliases(), 0);
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_branches(), 0);
}
