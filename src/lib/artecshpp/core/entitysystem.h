#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "ientitylistener.h"


namespace artecshpp {
namespace core {


class EntitySystem : public IEntityListener
{
public:
	EntitySystem();
	~EntitySystem();

    // IEntityListener interface
    void entityAdded(Entity e) override;
    void entityRemoved(Entity e) override;

private:
    bool m_activeEntities;

};


}}


#endif // ENTITYSYSTEM_H
