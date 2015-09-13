#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <cstdio>
#include <utility>
#include <ostream>
#include <bitset>


/***** OBJECT SOURCES *****************************************************/
template <typename T>
struct GET {
	static T getObj() { return T{}; }
};

template<>
struct GET<int> {
	static int getObj() { return 3; }
};

template<>
struct GET<double> {
	static double getObj() { return 3.14159268; }
};

template<>
struct GET<std::string> {
	static std::string getObj() { return "amastring"; }
};

template <typename Type>
class ObjectHandle {
public:
	Type duget() {
		return GET<Type>::getObj();
	}
};

typedef std::bitset<64> ComponentBits;
typedef std::uint32_t ctflags_t;
typedef std::uint32_t ctindex_t;

class ComponentTraits
{
public:
	template <typename T>
	static ctflags_t getFlag() {
		static const ctflags_t flag = pow(2, ComponentTraits::getIndex<T>());
		return flag;
	}

	template <typename T>
	static ctflags_t getIndex() {
		static const ctindex_t index = nextTypeIndex++;
		return index;
	}

	static ctflags_t getNumComponents() {
		return nextTypeIndex;
	}

private:
	static ctflags_t nextTypeIndex;
};


template <typename... Rest>
struct ComponentBitsBuilder;

template <typename First, typename... Rest>
struct ComponentBitsBuilder<First, Rest...>
{
	static ComponentBits buildBits()
	{
		ComponentBits b;
		b.set(ComponentTraits::getIndex<First>());
		return b | ComponentBitsBuilder<Rest...>::buildBits();
	}
};

template <>
struct ComponentBitsBuilder<>
{
	static ComponentBits buildBits()
	{
		return ComponentBits();
	}
};

class Entity {
public:
	typedef std::uint64_t eid_t;

	Entity( eid_t id )
		: m_id(id) { }

	eid_t getID() {
		return m_id;
	}

	void print() {
		std::cout << "MYID: " << m_id << "\n";
	}
private:
	eid_t m_id;
};

/**************************************************************************/
template <typename T>
struct M2 {
	std::string str() {
		std::stringstream ss;
		ss << "SpecMapper<" << GET<T>::getObj() << ">";
		return ss.str();
	}
};

template <typename T>
int getIndex() {
	return 0;
}

template <>
int getIndex<int>() {
	return 1;
}

/* *****************************************************/

template <typename T>
struct DefaultMemoryManager {

	static std::vector<T> s_data;

	static T* get(Entity e) {
		std::cout << "GETTING ID\n";
		return &(s_data[e.getID()]);
	}

	static void destroy(Entity e, T* t) {
		std::cout << "delete t; " << "(" << GET<T>::getObj() << ")\n";
	}

	template <typename... Args>
	static T& alloc(Entity e, Args&&... args) {
		std::cout << "T* t = new T(); return t; " << "(" << GET<T>::getObj() << ")\n";
		new (&(s_data[e.getID()])) T(args...);
		return s_data[e.getID()];
	}

};

template <typename T>
std::vector<T> DefaultMemoryManager<T>::s_data(10);

/*
template <typename T>
struct PoolMemoryManager {

	static void destroy(T* t) {
		std::cout << "pool.remove(t); " << "(" << GET<T>::getObj() << ")\n";
	}

	static T* alloc() {
		std::cout << "return pool.get(t)" << "(" << GET<T>::getObj() << ")\n";
		return nullptr;
	}

};

template <typename... Types>
using MultiPool = std::vector<std::tuple<Types...>> ;

template <typename T, typename... Types>
struct MultiPoolMemoryManager {

	static MultiPool<Types...> instance;

	static T* alloc() {
		std::cout << "multipool.alloc!\n";
		return nullptr;
	}

	static void destroy(T* t) {
		std::cout << "multipool.destroy!\n";
	}

};
*/

template <typename C>
struct MemoryStrategy {
	typedef DefaultMemoryManager<C> MemoryManager;
};

/*
template <>
struct MemoryStrategy<double> {
	typedef MultiPoolMemoryManager<double, float, short> MemoryManager;
	typedef DefaultMapper<double> Mapper;
};

template <>
struct MemoryStrategy<int> {
	typedef PoolMemoryManager<int> MemoryManager;
	typedef DefaultMapper<int> Mapper;
};
*/

/***************************************************** */
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
		typename MemoryStrategy<T>::MemoryManager::remove( e, t );
	}

private:
	static Entity::eid_t s_lastID;
};

Entity::eid_t EntityManager::s_lastID = 0;

/*************************************************************************/
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

class A {
public:
	A() {}
	A(int i, double d) {

	}
};

std::ostream& operator<< (std::ostream& stream, const A& a) {
	return stream << "I'm an A!";
}

int main( int argc, char** argv ) {

	std::function<void(int&,std::string&)> foo2 = [](int& i, std::string& s) {
		i = 2; s = "bbb";
		std::cout << i << " " << s << std::endl;
		i = 3; s = "ccc";
		std::cout << i << " " << s << std::endl;
	};

	EntityManager em;
	Entity e = em.createEntity();

	int& i = em.create<int>(e);
	std::string& s = em.create<std::string>(e);
	A& a = em.create<A>(e, 1, 2.5);
	std::cout << i << " " << s << std::endl;
	i = 1; s = "aaa";
	std::cout << i << " " << s << std::endl;
	apply2(e, em, foo2);
	std::cout << i << " " << s << std::endl;

	/*
	MemoryStrategy<int>::MemoryManager mgrInt;
	MemoryStrategy<std::string>::MemoryManager mgrString;
	MemoryStrategy<double>::MemoryManager mgrDouble;

	maker<int>(e, mgrInt);
	maker<double>(e, mgrDouble);
	maker<std::string>(e, mgrString);

	deleter<int>(e, mgrInt);
	deleter<double>(e, mgrDouble);
	deleter<std::string>(e, mgrString);
	*/

	return 0;
}
