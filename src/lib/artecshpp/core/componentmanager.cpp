#include "componentmanager.h"
#include "engine.h"
#include "componenttraits.h"


namespace artecshpp {
namespace core {


ComponentManager::ComponentManager()
{
	this->m_components = new Component**[Engine::MAX_ENTITIES];
	for( size_t i = 0; i < Engine::MAX_ENTITIES; i++ )
	{
		this->m_components[i] = new Component*[Engine::MAX_COMPONENTS];
	}
}

ComponentManager::~ComponentManager()
{
}

template <typename ComponentType>
ComponentType* ComponentManager::addComponent(int entity, ComponentType* component)
{
	return this->m_components[entity][ComponentTraits::getIndex<ComponentType>()] = component;
}


template <typename ComponentType>
ComponentType* ComponentManager::getComponent( int entity )
{
	return this->m_components[entity][ComponentTraits::getIndex<ComponentType>()];
}


template <typename ComponentType>
void ComponentManager::removeComponent(int entity)
{
	this->m_components[entity][ComponentTraits::getIndex<ComponentType>()];
}


}}