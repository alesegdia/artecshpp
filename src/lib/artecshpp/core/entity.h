#pragma once

#include <cstdint>
#include <stack>
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>

#include <artecshpp/core/componenttraits.h>
#include <artecshpp/core/aspect.h>
#include <artecshpp/core/memory.h>

#include "config.h"

namespace artecshpp {


typedef std::uint32_t eid_t;

class Entity {
public:
	typedef std::uint64_t eid_t;

	Entity( eid_t id )
		: m_id(id)
	{

	}

	eid_t getID() const
	{
		return m_id;
	}

	bool operator==(const Entity& other)
	{
		return m_id == other.getID();
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

class EntityManager;

class BaseComponentHelper {
public:
	BaseComponentHelper( EntityManager* em )
		: m_entityManager(em) {}
	virtual ~BaseComponentHelper() {}
	virtual void removeComponent(Entity e) = 0 ;

protected:
	EntityManager* m_entityManager;
};

template <typename Component>
class ComponentHelper : public BaseComponentHelper
{
public:
	ComponentHelper(EntityManager* em)
		: BaseComponentHelper(em) {}
	void removeComponent( Entity e ) override ;

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
		ctflags_t index = ComponentTraits::getIndex<T>();

		m_componentHelpers.resize(index, nullptr);
		if( m_componentHelpers[index] == nullptr )
		{
			m_componentHelpers[index] = new ComponentHelper<T>(this);
		}

		m_entityBits[e.getID()] |= ComponentBitsBuilder<T>::buildBits();
		ensurePoolSize<T>(e.getID());
		T* c = (static_cast<T*>(m_componentPools[index]->get(e.getID())));
		new (c) T;
		return *c;
	}

	template <typename T>
	T& getComponent(Entity e)
	{
		return *(static_cast<T*>(m_componentPools[ComponentTraits::getIndex<T>()]->get(e.getID())));
	}

	template <typename T>
	void removeComponent(Entity e)
	{
		ctflags_t index = ComponentTraits::getIndex<T>();
		m_componentPools[index]->destroy(e.getID());
	}

	bool hasComponent( Entity e, int index )
	{
		return m_entityBits[e.getID()].test(index);
	}

	void destroy(Entity e)
	{
		for( int i = 0; i < m_componentHelpers.size(); i++ )
		{
			if( hasComponent(e, i) )
			{
				m_componentHelpers[i]->removeComponent(e);
			}
		}
		m_freeIDs.push(e.getID());

		// insanely slow, will check this later
		m_alive.erase(std::remove(m_alive.begin(), m_alive.end(), e), m_alive.end());
	}

	std::vector<Entity>& alive()
	{
		return m_alive;
	}

	ComponentBits getBits( Entity e )
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

	std::vector<ComponentBits> m_entityBits{10};
	std::vector<BasePool*> m_componentPools;
	std::vector<IEntityListener*> m_observers;
	std::vector<Entity> m_alive; // usar más adelante sistema de versiones (dirty aumentativo (?))
	std::stack<Entity::eid_t> m_freeIDs;
	std::vector<BaseComponentHelper*> m_componentHelpers;

	template <typename Component>
	void ensurePool()
	{
		size_t component_index = ComponentTraits::getIndex<Component>();
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
		size_t component_index = ComponentTraits::getIndex<Component>();
		if( m_componentPools[component_index]->size() < e.getID() )
		{
			m_componentPools[component_index]->expand( e.getID() );
		}
	}

	void addListener(IEntityListener* obs)
	{
		this->m_observers.push_back(obs);
	}


};

template<typename Component>
void ComponentHelper<Component>::removeComponent(Entity e)
{
	m_entityManager->removeComponent<Component>(e);
}



}

