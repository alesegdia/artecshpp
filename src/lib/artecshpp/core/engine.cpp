#include "engine.h"

#include "componenttraits.h"

namespace artecshpp {
namespace core {


Engine::Engine()
{

}

Engine::~Engine()
{

}


Entity* Engine::addEntity( Entity* e )
{
	return e;
}


/**** EIDPool definition ****/
Engine::EIDPool::EIDPool()
{
	for( eid_t i = 0; i < Config::MAX_ENTITIES; i++ )
	{
		checkIn(i);
	}
}

void Engine::EIDPool::checkIn( eid_t id )
{
	m_eidStack.push( id );
}

eid_t Engine::EIDPool::checkOut()
{
	eid_t ret = m_eidStack.top();
	m_eidStack.pop();
	return ret;
}


}}

