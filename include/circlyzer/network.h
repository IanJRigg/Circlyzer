#ifndef NETWORK_H
#define NETWORK_H

#include <map>
#include <set>
#include <memory>
#include <string>
#include <vector>
#include <set>

#include "component.h"

namespace Circlyzer
{
struct Branch;
struct Node;

enum class Entity_Type
{
    Branch,
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

struct Branch : public Unique_Entity
{
    virtual ~Branch() = default;

    std::unique_ptr<Component> component;
    std::vector<uint32_t> nodes;
};

struct Node : public Unique_Entity
{
    Node(const uint32_t uid, const std::string& alias, const Entity_Type type) :
        Unique_Entity(uid, alias, type),
        branches { }
    {

    }

    virtual ~Node() = default;

    std::set<uint32_t> branches;
};

class Network
{
public:
    Network();
    virtual ~Network() = default;

    // Create Functions
    uint32_t create_node(const std::string& alias="");
    uint32_t create_branch(std::unique_ptr<Component> component, const std::string& alias="");

    // Read Functions
    const Component& get_component(uint32_t uid) const;
    const Component& get_component(const std::string& alias) const;

    // Update Functions
    void create_connection_between(uint32_t node_uid, uint32_t branch_uid);
    void delete_connection_between(uint32_t node_uid, uint32_t branch_uid);

    void create_connection_between(const std::string& node_alias, 
                                   const std::string& branch_alias);

    void delete_connection_between(const std::string& node_alias,
                                   const std::string& branch_alias);

    void update_alias(uint32_t uid, const std::string& new_alias);
    void update_alias(const std::string& alias, const std::string& new_alias);

    // TODO: add logic to update components

    void destroy_entity(uint32_t uid);
    void destroy_entity(const std::string& alias);

    // External Utility Functions
    uint32_t get_number_of_entities() const;
    uint32_t get_number_of_aliases() const;
    uint32_t get_number_of_nodes() const;
    uint32_t get_number_of_branches() const;
    uint32_t get_number_of_components() const;

private:
    // Internal utility functions
    uint32_t find_valid_uid() const;
    bool uid_does_not_exist(uint32_t uid) const;
    bool alias_does_not_exist(const std::string& alias) const;

    void check_for_new_meshes(uint32_t changed_branch_uid);
    void check_for_broken_meshes(uint32_t changed_branch_uid);

    std::map<uint32_t, Unique_Entity> entity_table;
    std::map<std::string, uint32_t> alias_to_id_table;

    uint32_t number_of_nodes;
    uint32_t number_of_branches;
    uint32_t number_of_components;

};

} // Namespace Circlyzer

#endif
