#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "entity.h"
#include "aspect.h"


namespace artecshpp {
namespace core {


class EntitySystem : public IEntityListener
{
public:
	virtual ~EntitySystem() = 0;

    // IEntityListener interface
    void entityAdded( Entity* e ) override;
    void entityRemoved( Entity* e ) override;

    bool checkEntity( Entity* e );

protected:
	Aspect aspect;

private:
	//rztl::UnorderedDynamicArray<Entity*> m_entities;

};


}}


#endif // ENTITYSYSTEM_H
