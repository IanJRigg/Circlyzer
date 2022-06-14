#ifndef NETWORK_H
#define NETWORK_H

#include <array>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "component.h"

struct Element;
struct Node;

enum class Entity_Type
{
    Element,
    Node
};

struct Unique_Entity
{
	virtual ~Unique_Entity() = default;

    uint32_t uid;
    std::string alias;
    Entity_Type type;
};

struct Element : Unique_Entity
{
	virtual ~Element() = default;

    std::unique_ptr<Component> component;
    std::array<std::weak_ptr<Node>, 2> terminals;
};

struct Node : Unique_Entity
{
	virtual ~Node() = default;

    std::vector<std::weak_ptr<Element>> elements;
};

class Network
{
public:
	Network();
	virtual ~Network() = default;

	// Create Functions
	uint32_t create_node(const std::string& alias="");
	uint32_t create_element(std::unique_ptr<Component> component, const std::string& alias="");

	// Read Functions
	std::weak_ptr<const Node> get_node(uint32_t uid) const;
	std::weak_ptr<const Element> get_element(uint32_t uid) const;

	std::weak_ptr<const Node> get_node(const std::string& alias) const;
	std::weak_ptr<const Element> get_element(const std::string& alias) const;

	// Update Functions
	void create_connection_between(uint32_t first_uid, uint32_t second_uid);
	void delete_connection_between(uint32_t first_uid, uint32_t second_uid);

	void create_connection_between(const std::string& first_alias, 
		                           const std::string& second_alias);

	void delete_connection_between(const std::string& first_alias,
		                           const std::string& second_alias);

	// Destroy Functions
    void destroy_node(uint32_t uid);
    void destroy_element(uint32_t uid);

    void destroy_node(const std::string& alias);
    void destroy_element(const std::string alias);

    // External Utility Functions
    uint32_t get_number_of_nodes() const;
    uint32_t get_number_of_elements() const;

private:
	// Internal utility functions
	uint32_t find_valid_uid() const;
	bool uid_does_not_exist(uint32_t uid) const;
	bool alias_does_not_exist(const std::string& alias) const;

	std::map<uint32_t, std::shared_ptr<Unique_Entity>> entity_table;
	std::map<std::string, uint32_t> alias_to_id_table;

	uint32_t number_of_nodes;
	uint32_t number_of_elements;
};

#endif
