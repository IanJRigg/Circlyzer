#include "circlyzer/network.h"

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
Network::Network() :
    entity_table(),
    alias_to_id_table(),
    number_of_nodes{ 0U },
    number_of_elements{ 0U }
{

}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
uint32_t Network::create_node(const std::string& alias)
{
    // TODO: don't populate the alias if given an empty alias
    // TODO: what to do if a duplicate alias is provided

	auto node = std::make_shared<Node>();
    node->uid = find_valid_uid();
	node->alias = alias;
	node->type = Entity_Type::Node;

	entity_table.insert({ node->uid, node });
    alias_to_id_table.insert({ alias, node->uid });
    ++number_of_nodes;

    return node->uid;
}

/**********************************************************************************************//**
 * \brief 
 * \param component
 * \param alias
 *************************************************************************************************/
uint32_t Network::create_element(std::unique_ptr<Component> component, const std::string& alias)
{
    // TODO: don't populate the alias if given an empty alias
    // TODO: what to do if a duplicate alias is provided

    auto element = std::make_shared<Element>();
    element->uid = find_valid_uid();
    element->alias = alias;
    element->type = Entity_Type::Element;
    element->component = std::move(component);

    entity_table.insert({ element->uid, element });
    alias_to_id_table.insert({ alias, element->uid });
    
    ++number_of_elements;

    return element->uid;
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
const Component& Network::get_component(const uint32_t uid) const
{
    if(uid_does_not_exist(uid))
    {
        throw std::out_of_range("Invalid uid provided: " + std::to_string(uid));
    }

    const auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Element)
    {
        throw std::runtime_error("Invalid entity request. Please request an element");
    }

    const auto element = dynamic_cast<const Element&>(*entity_ptr);

    return *(element.component);
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
const Component& Network::get_component(const std::string& alias) const
{
    if(alias_does_not_exist(alias))
    {
        throw std::out_of_range("Invalid alias provided: " + alias);
    }

    return get_component(alias_to_id_table.at(alias));
}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const uint32_t first_uid, const uint32_t second_uid)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const uint32_t first_uid, const uint32_t second_uid)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const std::string& first_alias,
                                        const std::string& second_alias)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const std::string& first_alias,
	                                    const std::string& second_alias)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
void Network::destroy_node(const uint32_t uid)
{
    if(uid_does_not_exist(uid))
    {
        return;
    }

    // TODO: Merge the Node and Element paths into one set of functions
    auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Node)
    {
        return;
    }

    // Retrieve the alias prior to deletion. This is so we can update alias_to_uid correctly
    const auto alias = entity_ptr->alias;

    // Cast down to the node object to ensure all destructors get called
    auto node_ptr = dynamic_pointer_cast<Element>(entity_ptr);
    node_ptr.reset();

    entity_table.erase(uid);
    alias_to_id_table.erase(alias);

    --number_of_nodes;
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
void Network::destroy_element(const uint32_t uid)
{
    if(uid_does_not_exist(uid))
    {
        return;
    }

    // TODO: Merge the Node and Element paths into one set of functions
    auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Element)
    {
        return;
    }

    // Retrieve the alias prior to deletion. This is so we can update alias_to_uid correctly
    const auto alias = entity_ptr->alias;

    // Cast down to the element object to ensure all destructors get called
    auto element_ptr = dynamic_pointer_cast<Element>(entity_ptr);
    element_ptr.reset();

    entity_table.erase(uid);
    alias_to_id_table.erase(alias);

    --number_of_elements;
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
void Network::destroy_node(const std::string& alias)
{
    alias_does_not_exist(alias);
    destroy_node(alias_to_id_table.at(alias));
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
void Network::destroy_element(const std::string alias)
{
    alias_does_not_exist(alias);
    destroy_element(alias_to_id_table.at(alias));
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
 * \brief Accessor for number_of_elements
 *************************************************************************************************/
uint32_t Network::get_number_of_elements() const
{
    return number_of_elements;
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
