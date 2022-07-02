#include "circlyzer/network.h"
#include "circlyzer/exceptions.h"

// Uncomment to disable assert()
// #define NDEBUG
#include <cassert>

namespace
{
    constexpr auto MAXIMUM_NUMBER_OF_NODES_FOR_ELEMENT = 2U;
    constexpr auto DEFAULT_ALIAS_LENGTH_LIMIT = 25U;
}

using namespace Circlyzer;

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
Network::Network() :
    entity_table(),
    alias_to_id_table(),
    number_of_nodes{ 0U },
    number_of_branches{ 0U },
    number_of_components { 0U }
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
    if(alias_to_id_table.find(alias) != alias_to_id_table.end())
    {
        throw Duplicate_Alias_Exception();
    }

    // Create the node
    Node node(find_valid_uid(), alias, Entity_Type::Node);

    // Insert all non-empty string aliases once they've been cleared for insertion
    if(alias.size() > 0)
    {
        alias_to_id_table.insert({ alias, node.uid });
    }

    // Insert the element
    entity_table.insert({ node.uid, node });
    ++number_of_nodes;

    return node.uid;
}

// /**********************************************************************************************//**
//  * \brief 
//  * \param component
//  * \param alias
//  *************************************************************************************************/
// uint32_t Network::create_element(std::unique_ptr<Component> component, const std::string& alias)
// {
//     if(component == nullptr)
//     {
//         throw Null_Component_Exception();
//     }

//     // Size check
//     if(alias.size() >= DEFAULT_ALIAS_LENGTH_LIMIT)
//     {
//         throw Invalid_Alias_Exception();
//     }

//     // Duplicate check
//     if(alias_to_id_table.find(alias) != alias_to_id_table.end())
//     {
//         throw Duplicate_Alias_Exception();
//     }

//     // Create the element
//     auto element = std::make_shared<Element>();
//     element->uid = find_valid_uid();
//     element->alias = alias;
//     element->type = Entity_Type::Element;
//     element->component = std::move(component);

//     // Insert all non-empty string aliases once they've been cleared for insertion
//     if(alias.size() > 0)
//     {
//         alias_to_id_table.insert({ alias, element->uid });
//     }

//     // Insert the element
//     entity_table.insert({ element->uid, element });
//     ++number_of_elements;

//     return element->uid;
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param uid 
//  *************************************************************************************************/
// const Component& Network::get_component(const uint32_t uid) const
// {
//     if(uid_does_not_exist(uid))
//     {
//         throw Non_Existant_UID_Exception();
//     }

//     const auto entity = entity_table.at(uid);
//     if(entity->type != Entity_Type::Element)
//     {
//         throw Wrong_Entity_Type_Exception();
//     }

//     const auto& element = dynamic_cast<const Element&>(entity_ptr);

//     return *(element.component);
// }

// /**********************************************************************************************//**
//  * \brief 
//  * \param alias 
//  *************************************************************************************************/
// const Component& Network::get_component(const std::string& alias) const
// {
//     if(alias_does_not_exist(alias))
//     {
//         throw Non_Existant_Alias_Exception();
//     }

//     return get_component(alias_to_id_table.at(alias));
// }

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
//     std::weak_ptr<Unique_Entity> node_weak_ptr = entity_table.at(node_uid);
//     std::weak_ptr<Unique_Entity> element_weak_ptr = entity_table.at(element_uid);

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
//     std::weak_ptr<Unique_Entity> node_weak_ptr = entity_table.at(node_uid);
//     std::weak_ptr<Unique_Entity> element_weak_ptr = entity_table.at(element_uid);

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

//     return create_connection_between(alias_to_id_table.at(node_alias),
//                                      alias_to_id_table.at(element_alias));
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

//     return create_connection_between(alias_to_id_table.at(node_alias),
//                                      alias_to_id_table.at(element_alias));
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

//     update_alias(alias_to_id_table.at(alias), new_alias);
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

    auto entity = entity_table.at(uid);

    if(entity.type == Entity_Type::Node)
    {
        // auto node_ptr = dynamic_pointer_cast<Node>(entity);
        // node_ptr.reset();
        // --number_of_nodes;
    }
    else if(entity.type == Entity_Type::Branch)
    {
        // auto branch = dynamic_pointer_cast<Branch>(entity);
        // branch_ptr.reset();
        // --number_of_components;
    }
    else
    {
        assert((false) && "Invalid entity type discovered on destroy");
    }

    alias_to_id_table.erase(entity.alias);
    entity_table.erase(entity.uid);
}

// /**********************************************************************************************//**
//  * \brief 
//  * \param alias 
//  *************************************************************************************************/
// void Network::destroy_entity(const std::string& alias)
// {
//     if(alias_does_not_exist(alias))
//     {
//         return;
//     }

//     destroy_entity(alias_to_id_table.at(alias));
// }

/**********************************************************************************************//**
 * \brief Accessor for number_of_nodes
 *************************************************************************************************/
uint32_t Network::get_number_of_entities() const
{
    return entity_table.size();
}

/**********************************************************************************************//**
 * \brief Accessor for number_of_nodes
 *************************************************************************************************/
uint32_t Network::get_number_of_aliases() const
{
   return alias_to_id_table.size();
}

/**********************************************************************************************//**
 * \brief Accessor for number_of_nodes
 *************************************************************************************************/
uint32_t Network::get_number_of_nodes() const
{
    return number_of_nodes;
}

/**********************************************************************************************//**
 * \brief Accessor for number_of_branches
 *************************************************************************************************/
uint32_t Network::get_number_of_branches() const
{
    return number_of_branches;
}

/**********************************************************************************************//**
 * \brief Accessor for number_of_components
 *************************************************************************************************/
uint32_t Network::get_number_of_components() const
{
    return number_of_components;
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
    for(auto it = entity_table.cbegin(); it != entity_table.cend(); ++it)
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
    return (entity_table.find(uid) == entity_table.end());
}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
bool Network::alias_does_not_exist(const std::string& alias) const
{
    return (alias_to_id_table.find(alias) == alias_to_id_table.end());
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