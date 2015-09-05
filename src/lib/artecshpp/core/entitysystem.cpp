#include "entitysystem.h"

#include "engine.h"

namespace artecshpp {
namespace core {


EntitySystem::~EntitySystem()
{
}


void EntitySystem::entityAdded( Entity* e )
{
	if( checkEntity(e) )
	{
		m_entities.add(e);
	}
}

void EntitySystem::entityRemoved( Entity* e )
{

}

bool EntitySystem::checkEntity( Entity* e )
{
	return true;
}


}}
