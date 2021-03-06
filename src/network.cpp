#include "circlyzer/network.h"
#include "circlyzer/exceptions.h"

// uncomment to disable assert()
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
    number_of_branches{ 0U }
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
    auto node = std::make_shared<Node>();
    node->uid = find_valid_uid();
    node->alias = alias;
    node->type = Entity_Type::Node;

    // Insert all non-empty string aliases once they've been cleared for insertion
    if(alias.size() > 0)
    {
        alias_to_id_table.insert({ alias, node->uid });
    }

    // Insert the branch
    entity_table.insert({ node->uid, node });
    ++number_of_nodes;

    return node->uid;
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
    if(alias_to_id_table.find(alias) != alias_to_id_table.end())
    {
        throw Duplicate_Alias_Exception();
    }

    // Create the branch
    auto branch = std::make_shared<Branch>();
    branch->uid = find_valid_uid();
    branch->alias = alias;
    branch->type = Entity_Type::Branch;
    branch->component = std::move(component);

    // Insert all non-empty string aliases once they've been cleared for insertion
    if(alias.size() > 0)
    {
        alias_to_id_table.insert({ alias, branch->uid });
    }

    // Insert the branch
    entity_table.insert({ branch->uid, branch });
    ++number_of_branches;

    return branch->uid;
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

    const auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Branch)
    {
        throw Wrong_Entity_Type_Exception();
    }

    const auto& branch = dynamic_cast<const Branch&>(*entity_ptr);

    return *(branch.component);
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

    return get_component(alias_to_id_table.at(alias));
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

    // Self references are forbidden
    if(node_uid == branch_uid)
    {
        return;
    }

    // Make sure that the two entities are a node and a component
    std::weak_ptr<Unique_Entity> node_weak_ptr = entity_table.at(node_uid);
    std::weak_ptr<Unique_Entity> branch_weak_ptr = entity_table.at(branch_uid);

    if(node_weak_ptr.expired() || branch_weak_ptr.expired())
    {
        assert((false) && "Internal pointer index has been corrupted!");
    }

    auto node_ptr = node_weak_ptr.lock();
    auto branch_ptr = node_weak_ptr.lock();

    if((node_ptr->type != Entity_Type::Node) && (branch_ptr->type != Entity_Type::Branch))
    {
        // Node and Branch weren't provided
        return;
    }

    auto& node = dynamic_cast<Node&>(*node_ptr);
    auto& branch = dynamic_cast<Branch&>(*branch_ptr);

    // Assign the connection to the first open terminal
    if(branch.nodes.size() < MAXIMUM_NUMBER_OF_NODES_FOR_ELEMENT)
    {
        branch.nodes.emplace_back(node_uid);
    }
    else if(branch.nodes.size() > MAXIMUM_NUMBER_OF_NODES_FOR_ELEMENT)
    {
        // No place for the new connection
        return;
    }
    else
    {
        assert((false) && "An branch has been connected to too many nodes!");
    }

    node.branches.emplace(branch_uid);

    // Return success
    return;
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

    // Self references are forbidden
    if(node_uid == branch_uid)
    {
        return;
    }

    // Make sure that the two entities are a node and a component
    std::weak_ptr<Unique_Entity> node_weak_ptr = entity_table.at(node_uid);
    std::weak_ptr<Unique_Entity> branch_weak_ptr = entity_table.at(branch_uid);

    if(node_weak_ptr.expired() || branch_weak_ptr.expired())
    {
        assert((false) && "Internal pointer index has been corrupted!");
    }

    auto node_ptr = node_weak_ptr.lock();
    auto branch_ptr = node_weak_ptr.lock();

    if((node_ptr->type != Entity_Type::Node) && (branch_ptr->type != Entity_Type::Branch))
    {
        // Node and Branch weren't provided
        return;
    }

    auto& node = dynamic_cast<Node&>(*node_ptr);
    auto& branch = dynamic_cast<Branch&>(*branch_ptr);

    if(branch.nodes.size() > 0 && branch.nodes.at(0) == node_uid)
    {
        branch.nodes.erase(branch.nodes.begin());
    }
    else if(branch.nodes.size() > 1 && branch.nodes.at(1) == node_uid)
    {
        branch.nodes.erase(std::next(branch.nodes.begin()));
    }
    else
    {
        // TODO: What to do here. If it's not here, but passed the checks, this may be an assert
        return;
    }

    node.branches.erase(branch_uid);
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

    return create_connection_between(alias_to_id_table.at(node_alias),
                                     alias_to_id_table.at(branch_alias));
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

    return create_connection_between(alias_to_id_table.at(node_alias),
                                     alias_to_id_table.at(branch_alias));
}

/**********************************************************************************************//**
 * \brief 
 * \param uid
 * \param new_alias 
 *************************************************************************************************/
void Network::update_alias(const uint32_t uid, const std::string& new_alias)
{
    if(uid_does_not_exist(uid))
    {
        return;
    }

    // TODO: Update the alias
}

/**********************************************************************************************//**
 * \brief 
 * \param alias
 * \param new_alias 
 *************************************************************************************************/
void Network::update_alias(const std::string& alias, const std::string& new_alias)
{
    if(alias_does_not_exist(alias))
    {
        return;
    }

    update_alias(alias_to_id_table.at(alias), new_alias);
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
void Network::destroy_entity(const uint32_t uid)
{
    if(uid_does_not_exist(uid))
    {
        return;
    }

    auto entity_ptr = entity_table.at(uid);
    const auto alias = entity_ptr->alias;
    const auto type = entity_ptr->type;

    if(type == Entity_Type::Node)
    {
        auto node_ptr = dynamic_pointer_cast<Node>(entity_ptr);
        node_ptr.reset();
        --number_of_nodes;
    }
    else if(type == Entity_Type::Branch)
    {
        auto branch_ptr = dynamic_pointer_cast<Branch>(entity_ptr);
        branch_ptr.reset();
        --number_of_branches;
    }
    else
    {
        assert((false) && "Invalid entity type discovered on destroy");
    }

    entity_table.erase(uid);
    alias_to_id_table.erase(alias);
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

    destroy_entity(alias_to_id_table.at(alias));
}

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
 * \param alias 
 *************************************************************************************************/
bool Network::uid_does_not_exist(const uint32_t uid) const
{
    return (entity_table.find(uid) == entity_table.end());
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
bool Network::alias_does_not_exist(const std::string& alias) const
{
    return (alias_to_id_table.find(alias) == alias_to_id_table.end());
}
