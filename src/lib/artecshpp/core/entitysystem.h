#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <rztl/unordereddynamicarray.h>

#include "ientitylistener.h"
#include "entity.h"


namespace artecshpp {
namespace core {


class EntitySystem : public IEntityListener
{
public:
	virtual ~EntitySystem() = 0;

    // IEntityListener interface
    void entityAdded(const Entity& e) override;
    void entityRemoved(const Entity& e) override;

    bool checkEntity( const Entity& e );

private:
	rztl::UnorderedDynamicArray<Entity> m_entities;

};


}}


#endif // ENTITYSYSTEM_H
