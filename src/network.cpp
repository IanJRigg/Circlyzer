#include "circlyzer/network.h"
#include "circlyzer/exceptions.h"

#include <queue>

// Uncomment to disable assert()
// #define NDEBUG
#include <cassert>

namespace
{
    constexpr auto DEFAULT_ALIAS_LENGTH_LIMIT = 25U;
}

using namespace Circlyzer;

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
Network::Network() :
    entity_lookup(),
    node_table(),
    branch_table(),
    alias_lookup(),
    mesh_table()
{

}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
uint32_t Network::create_node(const std::string& alias)
{
    // Size check
    if(alias.size() >= DEFAULT_ALIAS_LENGTH_LIMIT)
    {
        throw Invalid_Alias_Exception();
    }

    // Duplicate check
    if(alias_lookup.find(alias) != alias_lookup.end())
    {
        throw Duplicate_Alias_Exception();
    }

    // Create the node
    Node node(find_valid_uid(), alias, Entity_Type::Node);

    // Insert all non-empty string aliases once they've been cleared for insertion
    if(alias.size() > 0)
    {
        alias_lookup.insert({ alias, node.uid });
    }

    // Insert the element
    entity_lookup.insert({ node.uid, Entity_Type::Node });
    node_table.insert({ node.uid, node });

    return node.uid;
}

/**********************************************************************************************//**
 * \brief 
 * \param component
 * \param alias
 *************************************************************************************************/
uint32_t Network::create_branch(std::unique_ptr<Component> component, const std::string& alias)
{
    if(component == nullptr)
    {
        throw Null_Component_Exception();
    }

    // Size check
    if(alias.size() >= DEFAULT_ALIAS_LENGTH_LIMIT)
    {
        throw Invalid_Alias_Exception();
    }

    // Duplicate check
    if(alias_lookup.find(alias) != alias_lookup.end())
    {
        throw Duplicate_Alias_Exception();
    }

    // Create the element
    Branch branch(find_valid_uid(), alias, Entity_Type::Branch, std::move(component));

    // Insert all non-empty string aliases once they've been cleared for insertion
    if(alias.size() > 0)
    {
        alias_lookup.insert({ alias, branch.uid });
    }

    // Insert the element
    entity_lookup.insert({ branch.uid, Entity_Type::Branch });
    auto [it, _] = branch_table.insert({ branch.uid, std::move(branch) });

    return it->first;
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
const Component& Network::get_component(const uint32_t uid) const
{
    if(uid_does_not_exist(uid))
    {
        throw Non_Existant_UID_Exception();
    }

    const auto entity_type = entity_lookup.at(uid);
    if(entity_type != Entity_Type::Branch)
    {
        throw Wrong_Entity_Type_Exception();
    }

    return *(branch_table.at(uid).component.get());
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
const Component& Network::get_component(const std::string& alias) const
{
    if(alias_does_not_exist(alias))
    {
        throw Non_Existant_Alias_Exception();
    }

    return get_component(alias_lookup.at(alias));
}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const uint32_t node_uid, const uint32_t branch_uid)
{
    // Make sure both UIDs actually exist
    if(uid_does_not_exist(node_uid) || uid_does_not_exist(branch_uid))
    {
        return;
    }

    // Both entities need to exist
    if(node_does_not_exist(node_uid) || branch_does_not_exist(branch_uid))
    {
        return;
    }

    // Self references are forbidden
    if(node_uid == branch_uid)
    {
        return;
    }

    // Make sure that the two entities are a node and a component
    auto& node = node_table.at(node_uid);
    auto& branch = branch_table.at(branch_uid);

    // Assign the connection to the first open terminal
    if(branch.nodes.size() < 1)
    {
        branch.nodes.at(0) = node.uid;
    }
    else if(branch.nodes.size() < 2)
    {
        branch.nodes.at(1) = node.uid;
    }
    else
    {
        // No place for the new connection
        return;
    }

    node.branches.insert(branch_uid);

    // Assess loops in the network
    check_for_new_meshes(branch_uid);
}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const uint32_t node_uid, const uint32_t branch_uid)
{
    // Make sure both UIDs actually exist
    if(uid_does_not_exist(node_uid) || uid_does_not_exist(branch_uid))
    {
        return;
    }

    // Both entities need to exist
    if(node_does_not_exist(node_uid) || branch_does_not_exist(branch_uid))
    {
        return;
    }

    // Self references are forbidden
    if(node_uid == branch_uid)
    {
        return;
    }

    // Make sure that the two entities are a node and a component
    auto& node = node_table.at(node_uid);
    auto& branch = branch_table.at(branch_uid);

    // Remove the connection in the branch
    if(branch.nodes.front() == node_uid)
    {
        branch.nodes.erase(branch.nodes.begin());
    }
    else if(branch.nodes.back() == node_uid)
    {
        branch.nodes.pop_back();
    }
    else
    {
        // The requested UID doesn't exist back here
        return;
    }

    // Remove the branch from the node
    node.branches.erase(branch_uid);

    // Assess loops in the network
    check_for_broken_meshes(branch_uid);
}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const std::string& node_alias,
                                        const std::string& branch_alias)
{
    if(alias_does_not_exist(node_alias) || alias_does_not_exist(branch_alias))
    {
        return;
    }

    return create_connection_between(alias_lookup.at(node_alias),
                                     alias_lookup.at(branch_alias));
}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const std::string& node_alias,
                                        const std::string& branch_alias)
{
    if(alias_does_not_exist(node_alias) || alias_does_not_exist(branch_alias))
    {
        return;
    }

    return create_connection_between(alias_lookup.at(node_alias),
                                     alias_lookup.at(branch_alias));
}

// /**********************************************************************************************//**
//  * \brief 
//  * \param uid
//  * \param new_alias 
//  *************************************************************************************************/
// void Network::update_alias(const uint32_t uid, const std::string& new_alias)
// {
//     if(uid_does_not_exist(uid))
//     {
//         return;
//     }

//     // TODO: Update the alias
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param alias
//  * \param new_alias 
//  *************************************************************************************************/
// void Network::update_alias(const std::string& alias, const std::string& new_alias)
// {
//     if(alias_does_not_exist(alias))
//     {
//         return;
//     }

//     update_alias(alias_lookup.at(alias), new_alias);
// }

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
void Network::destroy_entity(const uint32_t uid)
{
    if(uid_does_not_exist(uid))
    {
        return;
    }

    std::string alias = "";
    switch(entity_lookup.at(uid))
    {
        case Entity_Type::Node:
            alias = node_table.at(uid).alias;
            node_table.erase(uid);
            break;

        case Entity_Type::Branch:
            alias = branch_table.at(uid).alias;
            branch_table.erase(uid);
            break;

        default:
            assert((false) && "Invalid entity type discovered on destroy");
            break;
    }
    
    if(alias.size() > 0)
    {
        alias_lookup.erase(alias);
    }

    entity_lookup.erase(uid);
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
void Network::destroy_entity(const std::string& alias)
{
    if(alias_does_not_exist(alias))
    {
        return;
    }

    destroy_entity(alias_lookup.at(alias));
}

/**********************************************************************************************//**
 * \brief Accessor for the current number of entities
 *************************************************************************************************/
uint32_t Network::get_number_of_entities() const
{
    return entity_lookup.size();
}

/**********************************************************************************************//**
 * \brief Accessor for the current number of aliases
 *************************************************************************************************/
uint32_t Network::get_number_of_aliases() const
{
   return alias_lookup.size();
}

/**********************************************************************************************//**
 * \brief Accessor for the current number of nodes
 *************************************************************************************************/
uint32_t Network::get_number_of_nodes() const
{
    return node_table.size();
}

/**********************************************************************************************//**
 * \brief Accessor for the current number of branches
 *************************************************************************************************/
uint32_t Network::get_number_of_branches() const
{
    return branch_table.size();
}


/**********************************************************************************************//**
 * \brief Iterates over the entity table. Since the std::map is stored in sorted order, the
 *        iteration will happen in sorted order. If N uids have been reserved, and they were all
 *        allocated in perfect serial order, this function will return N + 1. If a gap exists in
 *        allocation, this function will find that gap before N+1 is reached.
 * 
 * \note  This may be a potential performance issue on larger networks. But for now, I'll ignore
 *        that as it is very unlikely.
 *************************************************************************************************/
uint32_t Network::find_valid_uid() const
{
    uint32_t attempted_id = 0U;
    for(auto it = entity_lookup.cbegin(); it != entity_lookup.cend(); ++it)
    {
        if(it->first < attempted_id)
        {
            break;
        }

        ++attempted_id;
    }

    return attempted_id;
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
bool Network::uid_does_not_exist(const uint32_t uid) const
{
    return (entity_lookup.find(uid) == entity_lookup.end());
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
bool Network::alias_does_not_exist(const std::string& alias) const
{
    return (alias_lookup.find(alias) == alias_lookup.end());
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
bool Network::node_does_not_exist(const uint32_t uid) const
{
    return (node_table.find(uid) == node_table.end());
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
bool Network::branch_does_not_exist(const uint32_t uid) const
{
    return (branch_table.find(uid) == branch_table.find(uid));
}

/**********************************************************************************************//**
 * /brief
 * /note Only one mesh can be added to the network at any given time (This needs to be proven)
 * /note An existing mesh can be split into multiple sub-loops
 *************************************************************************************************/
void Network::check_for_new_meshes(const uint32_t root_uid)
{
    std::queue<uint32_t> queue;
    queue.push(root_uid);

    while(!queue.empty())
    {
        auto uid = queue.front();
        queue.pop();

        if(node_table.find(uid) != node_table.end())
        {
            auto& node = node_table.at(uid);

        }
        else if(branch_table.find(uid) != branch_table.end())
        {
            auto& branch = branch_table.at(uid);
        }
    }
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
void Network::check_for_broken_meshes(const uint32_t root_uid)
{
    // Check each of the meshes for the given UID
    // If any meshes contain that UID, remove those meshes from use
    // If more than one mesh uses a UID, merge the meshes (Need to prove more than 2 meshes can exist with one node)
}