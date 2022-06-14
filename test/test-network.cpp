#include "gtest/gtest.h"
#include "circlyzer/network.h"
#include "circlyzer/component.h"
#include "circlyzer/units.h"

#include <memory>

TEST(Network, Constructor)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);
}

TEST(Network, NodeCreation)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);

    auto first_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_nodes(), 1);
    EXPECT_EQ(network.get_number_of_elements(), 0);
    EXPECT_EQ(first_uid, 0);

    auto second_uid = network.create_node();

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_nodes(), 2);
    EXPECT_EQ(network.get_number_of_elements(), 0);
    EXPECT_EQ(second_uid, 1);
}

TEST(Network, ElementCreation)
{
    Network network;

    // Check entity counts
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);

    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor));

    // Make sure that ownership has been correctly transfered away from resistor
    EXPECT_EQ(resistor.get(), nullptr);

    // Check entity counts and UID identity
    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 1);
    EXPECT_EQ(first_uid, 0);
}

TEST(Network, AccessComponentWithInvalidUid)
{
    Network network;
    EXPECT_THROW(network.get_component(0xDEADBEEFU), std::out_of_range);
}

TEST(Network, AccessComponentWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node();
    EXPECT_THROW(network.get_component(first_uid), std::runtime_error);
}

TEST(Network, SuccessfullyAccessComponent)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor));
    const auto& component = network.get_component(first_uid);

    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, 1.0_ohm);
}

TEST(Network, AccessComponentWithInvalidAlias)
{
    Network network;
    EXPECT_THROW(network.get_component("DEAD BEEF!"), std::out_of_range);
}

TEST(Network, AccessComponentViaAliasWithInvalidType)
{
    Network network;
    auto first_uid = network.create_node("DEAD BEEF!");
    EXPECT_THROW(network.get_component(first_uid), std::runtime_error);
}

TEST(Network, SuccessfullyAccessComponentViaAlias)
{
    Network network;
    auto resistor = std::make_unique<Resistor>(1.0_ohm);
    auto first_uid = network.create_element(std::move(resistor), "DEAD BEEF!");
    const auto& component = network.get_component("DEAD BEEF!");

    EXPECT_EQ(component.type, Component_Type::Resistor);

    const auto& resistor_reference = dynamic_cast<const Resistor&>(component);
    EXPECT_EQ(resistor_reference.resistance, 1.0_ohm);
}
