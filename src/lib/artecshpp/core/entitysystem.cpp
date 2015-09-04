#include "entitysystem.h"

#include "engine.h"

namespace artecshpp {
namespace core {


EntitySystem::EntitySystem()
{
    m_activeEntities = new bool[Engine::MAX_ENTITIES];
}

EntitySystem::~EntitySystem()
{
}


void EntitySystem::entityAdded(Entity e)
{

}

void EntitySystem::entityRemoved(Entity e)
{
}


}}
