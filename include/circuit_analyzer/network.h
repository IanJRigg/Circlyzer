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
    uint32_t uid;
    std::string alias;
    Entity_Type type;
};

struct Element : Unique_Entity
{
    std::unique_ptr<Component> component;
    std::array<std::weak_ptr<Node>, 2> terminals;
};

struct Node : Unique_Entity
{
    std::vector<std::weak_ptr<Element>> elements;
};

class Network
{
public:
	Network();
	virtual ~Network() = default;

	// Create Functions
	uint32_t create_node();
	uint32_t create_element();

	// Read Functions
	const Node& get_node(uint32_t uid) const;
	const Element& get_element(uint32_t uid) const;

	const Node& get_node(const std::string& alias) const;
	const Element& get_element(const std::string& alias) const;

	// Update Functions
	void create_connection_between(uint32_t first_entity, uint32_t second_entity);
	void delete_connection_between(uint32_t first_entity, uint32_t second_entity);

	void create_connection_between(const std::string& first_entity, 
		                           const std::string& second_entity);

	void delete_connection_between(const std::string& first_entity,
		                           const std::string& second_entity);

	// Destroy Functions
    void destroy_node(uint32_t uid);
    void destroy_element(uint32_t uid);

    void destroy_node(const std::string& alias);
    void destroy_element(const std::string alias);

private:
	std::map<uint32_t, std::shared_ptr<Unique_Entity>> entity_table;
	std::map<std::string, uint32_t> alias_to_id_table;

	uint32_t id_incrementer;
};

#endif
