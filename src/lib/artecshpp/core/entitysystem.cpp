#include "entitysystem.h"

#include "engine.h"

namespace artecshpp {
namespace core {


EntitySystem::~EntitySystem()
{
}


void EntitySystem::entityAdded(const Entity& e)
{
	if( checkEntity(e) )
	{
		m_entities.Add(e);
	}
}

void EntitySystem::entityRemoved(const Entity& e)
{

}

bool EntitySystem::checkEntity( const Entity& e )
{

}


}}
