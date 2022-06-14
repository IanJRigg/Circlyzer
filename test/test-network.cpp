#include "gtest/gtest.h"
#include "circuit_analyzer/network.h"

// uint32_t create_node(const std::string& alias);
// uint32_t create_element(std::unique_ptr<Component> component, const std::string& alias);
// std::weak_ptr<const Node> get_node(uint32_t uid) const;
// std::weak_ptr<const Element> get_element(uint32_t uid) const;
// std::weak_ptr<const Node> get_node(const std::string& alias) const;
// std::weak_ptr<const Element> get_element(const std::string& alias) const;
// void create_connection_between(uint32_t first_uid, uint32_t second_uid);
// void delete_connection_between(uint32_t first_uid, uint32_t second_uid);
// void create_connection_between(const std::string& first_alias, 
// 	                           const std::string& second_alias);
// void delete_connection_between(const std::string& first_alias,
// 	                           const std::string& second_alias);
// void destroy_node(uint32_t uid);
// void destroy_element(uint32_t uid);
// void destroy_node(const std::string& alias);
// void destroy_element(const std::string alias);
// uint32_t get_number_of_nodes() const;
// uint32_t get_number_of_elements() const;

TEST(Network, Constructor)
{
    Network network;

    EXPECT_EQ(network.get_number_of_nodes(), 0);
    EXPECT_EQ(network.get_number_of_elements(), 0);
}

TEST(Network, NodeCreation)
{
    Network network;

}
