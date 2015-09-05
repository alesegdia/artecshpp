#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <stack>

#include "component.h"
#include "config.h"

namespace artecshpp {
namespace core {


typedef std::uint32_t eid_t;

class Entity {
public:

	Entity ();
	virtual ~Entity ();

	void setID( eid_t eid );

	template <typename ComponentType>
    ComponentType* addComponent( ComponentType* component );

	template <typename ComponentType>
    void removeComponent();

	template <typename ComponentType>
    ComponentType* getComponent();

private:
	ComponentBits m_bits;
	Component* m_components[Config::MAX_COMPONENTS];
	eid_t m_eid;

};


}}


#endif // ENTITY_H
