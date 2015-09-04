#include "engine.h"

#include "componenttraits.h"

namespace artecshpp {
namespace core {


Engine::Engine()
	: m_bitsets(new std::bitset<MAX_COMPONENTS>[MAX_ENTITIES]),
	  m_componentManager(new ComponentManager())
{
}

Engine::~Engine()
{
	delete [] m_bitsets;
}


template <typename ComponentType>
ComponentType* Engine::addComponent( Entity entity, ComponentType* component)
{
	this->m_bitsets[entity].set(ComponentTraits::getIndex<ComponentType>());
	return this->m_componentManager->addComponent<ComponentType>(entity, component);
}

template <typename ComponentType>
void Engine::removeComponent( Entity entity )
{
	this->m_bitsets[entity].reset(ComponentTraits::getIndex<ComponentType>());
	this->m_componentManager->removeComponent<ComponentType>(entity);
}


}}

