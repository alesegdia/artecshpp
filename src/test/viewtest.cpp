#include <functional>
#include <iterator>
#include <iostream>

#include <artecshpp/core/entity.h>
#include <artecshpp/core/ientitylistener.h>

typedef artecshpp::core::Entity Entity;
struct ComponentBits {};
struct Aspect {};

template <typename T>
struct DefaultMemoryManager {

	static std::vector<T> s_data;

	static T* get(Entity e) {
		std::cout << "GETTING ID\n";
		return &(s_data[e.getID()]);
	}

	static void destroy(Entity e, T* t) {

	}

	template <typename... Args>
	static T& alloc(Entity e, Args&&... args) {
		new (&(s_data[e.getID()])) T(args...);
		return s_data[e.getID()];
	}

};

template <typename T>
std::vector<T> DefaultMemoryManager<T>::s_data(10);

struct EntityManager {

	std::vector<Entity>& alive() {
		return m_alive;
	}

	template <typename T>
	T& createComponent(Entity e) {
		return DefaultMemoryManager<T>::alloc(e);
	}
	
	template <typename T>
	T& getComponent(Entity e) {
		return *DefaultMemoryManager<T>::get(e);
	}

	std::vector<Entity> m_alive;

};

struct ForwardIteratorFactory {
	static inline std::vector<Entity>::iterator begin( std::vector<Entity>& entities ) {
		return entities.begin();
	}
	static inline std::vector<Entity>::iterator end( std::vector<Entity>& entities ) {
		return entities.end();
	}
};


/**** EntityFilter ************************************************/
struct AliveFilter { //: public artecshpp::core::IEntityObserver {
	AliveFilter(EntityManager& eMgr)
		: m_eMgr(eMgr) {}
	EntityManager m_eMgr;

	std::vector<Entity>& entities() {
		return m_eMgr.alive();
	}
};

//template <typename... Components>
struct StorageFilter {
	StorageFilter(EntityManager& eMgr)
		: m_eMgr(eMgr) {}
	EntityManager m_eMgr;
	
	std::vector<Entity>& entities() {
		return m_entities;
	}
	std::vector<Entity> m_entities;
};
/*******************************************************************/

/**** AspectChecker ************************************************/
struct BitChecker {
	BitChecker( const Aspect& a ) {
		
	}

	inline bool check( const ComponentBits& bits ) {
		return true;
	}
};

struct AlwaysTrueChecker {
	AlwaysTrueChecker( const Aspect& a ) {
	}

	inline bool check( const ComponentBits& bits ) {
		return true;
	}
};
/*******************************************************************/


template <class EntityFilter, class IteratorFactory>
struct View {
	View( EntityManager& emgr )
		: m_eMgr(emgr), m_filter(emgr) {
		if( std::is_same<EntityFilter, artecshpp::core::IEntityObserver>::value ) {
			//emgr.addListener(m_filter);
		}
	}
	
	// if is_same StorageFilter, remove observer from emgr

	EntityFilter m_filter;
	IteratorFactory m_itFactory;

	template <typename... Args>
	void each(std::function<void(Args&...)> f) {
		// ver forma para separar la creación del iterador
		// con la selección de las entidades
		//IteratorType it( = m_filter.begin( );
		// IteratorType es un iterador de entidades
		auto it = m_itFactory.begin( m_filter.entities() );
		while( it != m_itFactory.end( m_filter.entities() ) ) {
			f( *it, (m_eMgr.template getComponent<Args>(*it)) ... );
		}
	}

	template <typename... Args>
	void each(Entity e, std::function<void(Entity&,Args&...)> f) {
		f( e, (m_eMgr.template getComponent<Args>(e)) ... );
	}


	
	EntityManager m_eMgr;
};

/**** Systems **************************************************/
class BaseSystem {
public:
	virtual ~BaseSystem() = 0;
	virtual void process(Entity e) = 0;
};

BaseSystem::~BaseSystem() {
	
}

template <class A, template <typename> class T>
struct EntityView {};


#include <type_traits>

template <typename Derived, typename ViewType, typename... Args>
class System : public BaseSystem {
public:
	System( EntityManager& emgr )
		: m_emgr(emgr), m_view(emgr) { }
	~System() {
		
	}

	void process(Entity e) {
		m_view.template each<Args...>(e, static_cast<Derived*>(this)->function);
	}
	
private:
	ViewType m_view;	
	EntityManager m_emgr;
	
};

class SampleSystem : public System<
	SampleSystem,
	View<StorageFilter, ForwardIteratorFactory>,
	int, double, std::string> {
public:
	SampleSystem(EntityManager& emgr)
	: System<SampleSystem, View<StorageFilter, ForwardIteratorFactory>, int, double, std::string>::System(emgr) { }
	std::function<void(Entity& e, int& i, double& d, std::string& str)> function =
		[](Entity& e, int& i, double& d, std::string& s) {
			std::cout 	<< i << std::endl
						<< d << std::endl
						<< s << std::endl;
		};
};
/***************************************************************/
/*
world::process() {
	processAllSystems();

	for( gds : gravediggerSystems ) {
		gds.process();
	}

	template <T>
	gds.process() {
		for( e : deadentities ) {
			eMgr->deleteComponent<T>(e);
		}
	}

}
*/

int main( int argc, char** argv ) {
	EntityManager emgr;
	Entity e0(0);
	Entity e1(1);
	int& e1i = emgr.createComponent<int>(e0);
	double& e1d = emgr.createComponent<double>(e0);
	std::string& e1s = emgr.createComponent<std::string>(e0);
	SampleSystem ss(emgr);
	ss.process(e0);
	e1i = 3;
	e1d = 0.5;
	e1s = "olaqase";
	ss.process(e0);
	return 0;
}
