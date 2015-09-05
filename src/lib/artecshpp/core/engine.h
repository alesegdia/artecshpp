#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <bitset>

#include "config.h"
#include "entity.h"

namespace artecshpp {
namespace core {


class Engine
{
public:
	Engine();
	~Engine();

	Entity* addEntity( Entity* e );


private:

	class EIDPool {
	public:
		EIDPool();
		void checkIn(eid_t id);
		eid_t checkOut();
	private:
		std::stack<eid_t> m_eidStack;
	};

	EIDPool m_eidPool;
	Entity m_entities[Config::MAX_ENTITIES];


};


}}


#endif // ENGINE_H
