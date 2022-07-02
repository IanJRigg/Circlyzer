#include "circlyzer/network.h"
#include "circlyzer/exceptions.h"

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
    alias_lookup()
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

// /**********************************************************************************************//**
//  * \brief 
//  * \param first_entity
//  * \param second_entity 
//  *************************************************************************************************/
// void Network::create_connection_between(const uint32_t node_uid, const uint32_t element_uid)
// {
//     // Make sure both UIDs actually exist
//     if(uid_does_not_exist(node_uid) || uid_does_not_exist(element_uid))
//     {
//         return;
//     }

//     // Self references are forbidden
//     if(node_uid == element_uid)
//     {
//         return;
//     }

//     // Make sure that the two entities are a node and a component
//     std::weak_ptr<Unique_Entity> node_weak_ptr = entity_lookup.at(node_uid);
//     std::weak_ptr<Unique_Entity> element_weak_ptr = entity_lookup.at(element_uid);

//     if(node_weak_ptr.expired() || element_weak_ptr.expired())
//     {
//         assert((false) && "Internal pointer index has been corrupted!");
//     }

//     auto node_ptr = node_weak_ptr.lock();
//     auto element_ptr = node_weak_ptr.lock();

//     if((node_ptr->type != Entity_Type::Node) &&
//        (element_ptr->type != Entity_Type::Element))
//     {
//         // Node and Element weren't provided
//         return;
//     }

//     auto& node = dynamic_cast<Node&>(*node_ptr);
//     auto& element = dynamic_cast<Element&>(*element_ptr);

//     // Assign the connection to the first open terminal
//     if(element.nodes.front().expired())
//     {
//         element.nodes.at(0) = dynamic_pointer_cast<Node>(node_ptr);
//     }
//     else if(element.nodes.back().expired())
//     {
//         element.nodes.at(1) = dynamic_pointer_cast<Node>(node_ptr);
//     }
//     else
//     {
//         // No place for the new connection
//         return;
//     }

//     // node.elements.emplace_back(element_weak_ptr);

//     // Return success
//     return;
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param first_entity
//  * \param second_entity 
//  *************************************************************************************************/
// void Network::delete_connection_between(const uint32_t node_uid, const uint32_t element_uid)
// {
//     // Make sure both UIDs actually exist
//     if(uid_does_not_exist(node_uid) || uid_does_not_exist(element_uid))
//     {
//         return;
//     }

//     // Self references are forbidden
//     if(node_uid == element_uid)
//     {
//         return;
//     }

//     // Make sure that the two entities are a node and a component
//     std::weak_ptr<Unique_Entity> node_weak_ptr = entity_lookup.at(node_uid);
//     std::weak_ptr<Unique_Entity> element_weak_ptr = entity_lookup.at(element_uid);

//     if(node_weak_ptr.expired() || element_weak_ptr.expired())
//     {
//         assert((false) && "Internal pointer index has been corrupted!");
//     }

//     auto node_ptr = node_weak_ptr.lock();
//     auto element_ptr = node_weak_ptr.lock();

//     if((node_ptr->type != Entity_Type::Node) &&
//        (element_ptr->type != Entity_Type::Element))
//     {
//         // Node and Element weren't provided
//         return;
//     }

//     auto& node = dynamic_cast<Node&>(*node_ptr);
//     auto& element = dynamic_cast<Element&>(*element_ptr);
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param first_entity
//  * \param second_entity 
//  *************************************************************************************************/
// void Network::create_connection_between(const std::string& node_alias,
//                                         const std::string& element_alias)
// {
//     if(alias_does_not_exist(node_alias) || alias_does_not_exist(element_alias))
//     {
//         return;
//     }

//     return create_connection_between(alias_lookup.at(node_alias),
//                                      alias_lookup.at(element_alias));
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param first_entity
//  * \param second_entity 
//  *************************************************************************************************/
// void Network::delete_connection_between(const std::string& node_alias,
//                                         const std::string& element_alias)
// {
//     if(alias_does_not_exist(node_alias) || alias_does_not_exist(element_alias))
//     {
//         return;
//     }

//     return create_connection_between(alias_lookup.at(node_alias),
//                                      alias_lookup.at(element_alias));
// }

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
 * /brief
 *************************************************************************************************/
void Network::check_for_new_meshes(const uint32_t changed_branch_uid)
{
    // If no loops exist, the first loop discovered will
    // NOTE: Only one mesh can be added to the network at any given time (This needs to be proven)
    // NOTE: An existing mesh can be split into multiple sub-loops however
    
    // DFS from the changed UID
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
void Network::check_for_broken_meshes(const uint32_t changed_branch_uid)
{
    // Check each of the meshes for the given UID
    // If any meshes contain that UID, remove those meshes from use
    // If more than one mesh uses a UID, merge the meshes (Need to prove more than 2 meshes can exist with one node)
}