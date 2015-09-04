#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <bitset>

#include "componentmanager.h"
#include "entity.h"

namespace artecshpp {
namespace core {


class Engine
{
public:
	static const int MAX_ENTITIES = 1024;
	static const int MAX_COMPONENTS = 64;

	Engine();
	~Engine();

	template <typename ComponentType>
    ComponentType* addComponent( Entity entity, ComponentType* component );
	
	template <typename ComponentType>
    void removeComponent( Entity entity );


private:
	std::bitset<MAX_COMPONENTS>* m_bitsets;
	std::shared_ptr<ComponentManager> m_componentManager;
	
};


}}


#endif // ENGINE_H
