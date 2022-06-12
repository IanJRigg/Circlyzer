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

struct Unique_Entity
{
    uint32_t uid;
    std::string alias;
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
	const Node& get_node(uint32_t uid) const;
	const Element& get_element(uint32_t uid) const;

	void create_connection_between(uint32_t first_entity, uint32_t second_entity);
	void delete_connection_between(uint32_t first_entity, uint32_t second_entity);

	void find_loops() const;

private:
	std::map<uint32_t, std::shared_ptr<Unique_Entity>> entity_table;
	std::map<std::string, uint32_t> alias_to_id_table;
};

#endif
