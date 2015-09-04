#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <memory>

#include "component.h"


namespace artecshpp {
namespace core {


/**
 * @class ComponentManager
 * @author alesegdia
 * @date 04/09/15
 * @file componentmanager.h
 * @brief this class purpose is to hold all components related to entities
 */
class ComponentManager
{
public:

	ComponentManager();
	~ComponentManager();

	/**
	 * @brief add a new component associated to an entity
	 * @param entity having the component
	 * @param component owned by the entity
	 * @return the same component for utility 
	 */
	template <typename ComponentType>
	ComponentType* addComponent( int entity, ComponentType* component );

	template <typename ComponentType>
	ComponentType* getComponent( int entity );

	/**
	 * @brief removes a component from an entity
	 * @param entity to remove the component from
	 */
	template <typename ComponentType>
	void removeComponent( int entity );


private:

	/**
     * @brief components from all entities 
	 */
	Component*** m_components; // MAX_ENTITIES * MAX_COMPONENTS

};

/**
 * Currently, the component storage is static
 * It can be made with a pool so that components are allocated in blocks increasable on demand
 *     i.e.: blocks with N MAX_COMPONENTS linked to each other,
 *           entity N block is N / num_entities_per_block + N % num_entities_per_block 
 *     i.e.: if addComponent entity is 512 and current max is 256, increase until 512.
 */

}}

#endif // COMPONENTMANAGER_H
