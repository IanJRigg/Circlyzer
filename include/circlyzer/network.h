#ifndef NETWORK_H
#define NETWORK_H

#include <array>
#include <complex>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <vector>

#include "component.h"

namespace Circlyzer
{
struct Element;
struct Node;

enum class Entity_Type
{
    Element,
    Node
};

struct Unique_Entity
{
    Unique_Entity(const uint32_t uid, const std::string& alias, const Entity_Type type) :
        uid { uid },
        alias { alias },
        type { type }
    {
        
    }

    virtual ~Unique_Entity() = default;

    uint32_t uid;
    std::string alias;
    Entity_Type type;
};

struct Element : public Unique_Entity
{
    virtual ~Element() = default;

    std::unique_ptr<Component> component;
    std::array<uint32_t, 2> nodes;
};

struct Node : public Unique_Entity
{
    Node(const uint32_t uid, const std::string& alias, const Entity_Type type) :
        Unique_Entity(uid, alias, type),
        elements { }
    {

    }

    virtual ~Node() = default;

    std::set<uint32_t> elements;
};

class Network
{
public:
    Network();
    virtual ~Network() = default;

    // Create Functions
    uint32_t create_node(const std::string& alias="");
    uint32_t create_element(const Component& component, const std::string& alias="");

    // Read Functions
    const Component& get_component(uint32_t uid) const;
    const Component& get_component(const std::string& alias) const;

    // Update Functions
    void create_connection_between(uint32_t node_uid, uint32_t element_uid);
    void delete_connection_between(uint32_t node_uid, uint32_t element_uid);

    void create_connection_between(const std::string& node_alias, 
                                   const std::string& element_alias);

    void delete_connection_between(const std::string& node_alias,
                                   const std::string& element_alias);

    void update_alias(uint32_t uid, const std::string& new_alias);
    void update_alias(const std::string& alias, const std::string& new_alias);

    // TODO: add logic to update components

    void destroy_entity(uint32_t uid);
    void destroy_entity(const std::string& alias);

    // External Utility Functions
    uint32_t get_number_of_entities() const;
    uint32_t get_number_of_aliases() const;
    uint32_t get_number_of_nodes() const;
    uint32_t get_number_of_elements() const;

private:
    // Internal utility functions
    uint32_t find_valid_uid() const;
    bool uid_does_not_exist(uint32_t uid) const;
    bool alias_does_not_exist(const std::string& alias) const;

    std::map<uint32_t, Unique_Entity> entity_table;
    std::map<std::string, uint32_t> alias_to_id_table;

    uint32_t number_of_nodes;
    uint32_t number_of_elements;
};

} // Namespace Circlyzer

#endif
