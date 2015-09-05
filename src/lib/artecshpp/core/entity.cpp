#include <stdlib.h>

#include "entity.h"
#include "componenttraits.h"

namespace artecshpp {
namespace core {


template <typename ComponentType>
ComponentType* Entity::addComponent( ComponentType* component)
{
	size_t index = ComponentTraits::getIndex<ComponentType>();
	this->m_bitset.set(index);
	this->m_components[index] = component;
	return this->m_components[index];
}

template <typename ComponentType>
void Entity::removeComponent()
{
	size_t index = ComponentTraits::getIndex<ComponentType>();
	delete m_components[index];
	m_components[index] = nullptr;
}

template <typename ComponentType>
ComponentType* Entity::getComponent()
{
	size_t index = ComponentTraits::getIndex<ComponentType>();
	return m_components[index];
}


}}


