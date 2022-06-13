#include "circuit_analyzer/network.h"

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
Network::Network() :
    entity_table(),
    alias_to_id_table()
{

}

/**********************************************************************************************//**
 * \brief 
 *************************************************************************************************/
uint32_t Network::create_node(const std::string& alias="")
{
	auto node = std::make_shared<Node>();
    node->uid = find_valid_uid();
	node->alias = alias;
	node->type = Entity_Type::Node;

	entity_table.insert({ node->uid, node });

    return node->uid;
}

/**********************************************************************************************//**
 * \brief 
 * \param component
 * \param alias
 *************************************************************************************************/
uint32_t Network::create_element(std::unique_ptr<Component> component, const std::string& alias)
{
    auto element = std::make_shared<Element>();
    element->uid = find_valid_uid();
    element->alias = alias;
    element->type = Entity_Type::Element;
    element->component = std::move(component);

    entity_table.insert({ element->uid, element });

    return element->uid;
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
const Node& Network::get_node(const uint32_t uid) const
{
    if(entity_table.find(uid) == entity_table.end())
    {
        throw std::out_of_range("UID no found in the circuit");
    }

    const auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Node)
    {
        throw std::runtime_error("Invalid type requested");
    }

    return *(dynamic_pointer_cast<Node>(entity_ptr));
}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
const Element& Network::get_element(const uint32_t uid) const
{
    if(entity_table.find(uid) == entity_table.end())
    {
        throw std::out_of_range("UID no found in the circuit");
    }

    const auto entity_ptr = entity_table.at(uid);
    if(entity_ptr->type != Entity_Type::Element)
    {
        throw std::runtime_error("Invalid type requested");
    }

    return *(dynamic_pointer_cast<Element>(entity_ptr));
}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
const Node& Network::get_node(const std::string& alias) const
{

}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
const Element& Network::get_element(const std::string& alias) const
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const uint32_t first_entity, const uint32_t second_entity)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const uint32_t first_entity, const uint32_t second_entity)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::create_connection_between(const std::string& first_entity,
                                        const std::string& second_entity)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param first_entity
 * \param second_entity 
 *************************************************************************************************/
void Network::delete_connection_between(const std::string& first_entity,
	                                    const std::string& second_entity)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
void Network::destroy_node(const uint32_t uid)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param uid 
 *************************************************************************************************/
void Network::destroy_element(const uint32_t uid)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
void Network::destroy_node(const std::string& alias)
{

}

/**********************************************************************************************//**
 * \brief 
 * \param alias 
 *************************************************************************************************/
void Network::destroy_element(const std::string alias)
{

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
        	return attempted_id;
        }

        ++attempted_id;
	}
}
