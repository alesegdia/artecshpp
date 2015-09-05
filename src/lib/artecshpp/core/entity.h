#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <stack>
#include <bitset>

#include "component.h"
#include "config.h"

namespace artecshpp {
namespace core {


typedef std::uint32_t eid_t;

class Entity {
public:

	Entity ();
	virtual ~Entity ();

	template <typename ComponentType>
    ComponentType* addComponent( ComponentType* component );

	template <typename ComponentType>
    void removeComponent();

	template <typename ComponentType>
    ComponentType* getComponent();

private:
	std::bitset<Config::MAX_COMPONENTS> m_bitset;
	Component* m_components[Config::MAX_COMPONENTS];

};


}}


#endif // ENTITY_H
