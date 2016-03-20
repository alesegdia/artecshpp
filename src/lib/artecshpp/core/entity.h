#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <stack>
#include <functional>
#include <vector>
#include <iostream>

#include "config.h"

namespace artecshpp {
namespace core {


typedef std::uint32_t eid_t;

class Entity {
public:
	typedef std::uint64_t eid_t;

	Entity( eid_t id )
		: m_id(id) { }

	eid_t getID() {
		return m_id;
	}

private:
	eid_t m_id;
};


template <typename T>
struct DefaultMemoryManager {

	static std::vector<T> s_data;

	static T* get(Entity e) {
		std::cout << "GETTING ID\n";
		return &(s_data[e.getID()]);
	}

	static void destroy(Entity e, T* t) {
		std::cout << "delete t; " << "(" << ")\n";
	}

	template <typename... Args>
	static T& alloc(Entity e, Args&&... args) {
		std::cout << "T* t = new T(); return t; " << "(" << ")\n";
		new (&(s_data[e.getID()])) T(args...);
		return s_data[e.getID()];
	}

};

template <typename T>
std::vector<T> DefaultMemoryManager<T>::s_data(10);

template <typename C>
struct MemoryStrategy {
	typedef DefaultMemoryManager<C> MemoryManager;
};

class EntityManager {
public:
	Entity createEntity() {
		Entity e(s_lastID++);
		return e;
	}

	template <typename T>
	T* get(Entity e) {
		return MemoryStrategy<T>::MemoryManager::get(e);
	}

	template <typename T, typename... Args>
	T& create(Entity e, Args&&... args) {
		return MemoryStrategy<T>::MemoryManager::alloc(e, std::forward<Args>(args)...);
	}

	template <typename T>
	void remove(Entity e, T* t) {
		MemoryStrategy<T>::MemoryManager::destroy( e, t );
	}

	template <typename... Types>
	void apply2( Entity e, EntityManager em, std::function<void(Types&...)> f ) {
		f( *(em.template get<Types>(e))... );
	}

	template <typename T>
	void maker(Entity e, typename MemoryStrategy<T>::MemoryManager mm) {
		MemoryStrategy<T>::MemoryManager::alloc(e);
	}

	template <typename T>
	void deleter(Entity e, typename MemoryStrategy<T>::MemoryManager mm) {
		MemoryStrategy<T>::MemoryManager::destroy(e, nullptr);
	}

private:
	static Entity::eid_t s_lastID;
};




}}


#endif // ENTITY_H
