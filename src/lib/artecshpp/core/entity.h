#pragma once

#include <cstdint>
#include <stack>
#include <functional>
#include <vector>
#include <iostream>

#include <artecshpp/core/componenttraits.h>
#include <artecshpp/core/aspect.h>
#include <artecshpp/core/memory.h>

#include "config.h"

namespace artecshpp {
namespace core {


typedef std::uint32_t eid_t;

class Entity {
public:
	typedef std::uint64_t eid_t;

	Entity( eid_t id )
		: m_id(id)
	{

	}

	eid_t getID()
	{
		return m_id;
	}

private:
	eid_t m_id;

};


class IEntityListener
{
public:
	virtual ~IEntityListener() = 0 ;

	virtual void entityAdded( Entity* e ) = 0 ;
	virtual void entityRemoved( Entity* e ) = 0 ;

};


class EntityManager {
public:
	Entity createEntity()
	{
		Entity::eid_t id;
		if( !m_freeIDs.empty() )
		{
			id = m_freeIDs.top();
			m_freeIDs.pop();
		}
		else
		{
			id = s_lastID++;
		}
		return Entity(id);
	}

	template <typename T>
	T& createComponent(Entity e)
	{
		m_entityBits[e.getID()] |= artecshpp::core::ComponentBitsBuilder<T>::buildBits();
		ensurePoolSize<T>(e.getID());
		T* c = (static_cast<T*>(m_componentPools[artecshpp::core::ComponentTraits::getIndex<T>()]->get(e.getID())));
		new (c) T;
		return *c;
	}

	template <typename T>
	T& getComponent(Entity e)
	{
		return *(static_cast<T*>(m_componentPools[artecshpp::core::ComponentTraits::getIndex<T>()]->get(e.getID())));
	}

	template <typename T>
	void removeComponent(Entity e)
	{
		m_componentPools[ComponentTraits::getIndex<T>()]->destroy(e.getID());
	}

	std::vector<Entity>& alive()
	{
		return m_alive;
	}

	artecshpp::core::ComponentBits getBits( Entity e )
	{
		return m_entityBits[e.getID()];
	}

	void addEntity(Entity e)
	{
		m_alive.push_back(e);
	}

	int numObservers()
	{
		return m_observers.size();
	}

	template <typename T>
	void tryAddListener( typename std::enable_if<std::is_base_of<IEntityListener, T>::value, T>::type& t)
	{
		addListener(&t);
	}

	template <typename T>
	void tryAddListener( typename std::enable_if<!std::is_base_of<IEntityListener, T>::value, T>::type& t)
	{

	}

private:

	static Entity::eid_t s_lastID;

	std::vector<artecshpp::core::ComponentBits> m_entityBits{10};
	std::vector<BasePool*> m_componentPools;
	std::vector<IEntityListener*> m_observers;
	std::vector<Entity> m_alive; // usar más adelante sistema de versiones (dirty aumentativo (?))
	std::stack<Entity::eid_t> m_freeIDs;

	template <typename Component>
	void ensurePool()
	{
		size_t component_index = artecshpp::core::ComponentTraits::getIndex<Component>();
		if( m_componentPools.size() <= component_index )
		{
			m_componentPools.resize(component_index+1, nullptr);
		}

		if( m_componentPools[component_index] == nullptr )
		{
			m_componentPools[component_index] = new Pool<Component>();
		}
	}

	template <typename Component>
	void ensurePoolSize( Entity e )
	{
		ensurePool<Component>();
		size_t component_index = artecshpp::core::ComponentTraits::getIndex<Component>();
		if( m_componentPools[component_index]->size() < e.getID() )
		{
			m_componentPools[component_index]->expand( e.getID() );
		}
	}

	void addListener(IEntityListener* obs)
	{
		this->m_observers.push_back(obs);
	}

	void destroy(Entity e)
	{
		artecshpp::core::ComponentBits bits = m_entityBits[e.getID()];
		for( int i = 0; i < m_componentPools.size(); i++ )
		{
			BasePool *pool = m_componentPools[i];
			if( pool && bits.test(i) )
			{
				pool->destroy(e.getID());
			}
		}
		// remove from alive
	}


};


}}

