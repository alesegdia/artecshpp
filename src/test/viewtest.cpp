#include <functional>
#include <iterator>
#include <iostream>

#include <artecshpp/core/entity.h>
#include <artecshpp/core/ientitylistener.h>
#include <artecshpp/core/componenttraits.h>

typedef artecshpp::core::Entity Entity;
struct ComponentBits {};
struct Aspect {};

/**** Components Memory Management ********************/
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

/**** EntityManager ***************************************/
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
	void addEntity(Entity e) {
		m_alive.push_back(e);
	}
	
	template <typename T>
	void tryAddListener( typename std::enable_if<std::is_base_of<artecshpp::core::IEntityObserver, T>::value, T>::type& t)
	{
		addListener(&t);		
	}

	template <typename T>
	void tryAddListener( typename std::enable_if<!std::is_base_of<artecshpp::core::IEntityObserver, T>::value, T>::type& t)
	{
		std::cout << "foo\n";
	}

	void addListener(artecshpp::core::IEntityObserver* obs) {
		this->m_observers.push_back(obs);
	}

	std::vector<artecshpp::core::IEntityObserver*> m_observers;
	std::vector<Entity> m_alive; // usar más adelante sistema de versiones (dirty aumentativo (?))

};

/**** Iterator Factories ******************************************/

template <typename CheckerType>
struct CheckerIteratorFactory {
	CheckerType ct;
};

struct ForwardIteratorFactory {
	
	void setBits( const artecshpp::core::ComponentBits& bits ) {
		
	}
	
	static inline std::vector<Entity>::iterator begin( std::vector<Entity>& entities ) {
		return entities.begin();
	}
	
	static inline std::vector<Entity>::iterator end( std::vector<Entity>& entities ) {
		return entities.end();
	}
};


/**** EntityFilters ************************************************/

// pass to EntityFilters a template parameter indicating the bit checker

struct AliveFilter {
	AliveFilter(EntityManager& eMgr)
		: m_eMgr(eMgr) {}
	EntityManager m_eMgr;
	
	void setBits( const artecshpp::core::ComponentBits& bits ) {
		
	}
	
	std::vector<Entity>& entities() {
		return m_eMgr.alive();
	}
};

struct StorageFilter : public artecshpp::core::IEntityObserver {
	StorageFilter(EntityManager& eMgr)
		: m_eMgr(eMgr) {}
	EntityManager m_eMgr;
	
	void setBits( const artecshpp::core::ComponentBits& bits ) {
		
	}
	
	std::vector<Entity>& entities() {
		return m_entities;
	}
	
	void entityAdded(Entity* e) override {
		// if component bits ok
		m_entities.push_back(*e);
	}
	
	void entityRemoved(Entity* e) override {
		
	}
	
	
	std::vector<Entity> m_entities;
};

/**** AspectChecker ************************************************/
struct BitChecker {

	inline bool check( const Entity& e ) {
		return true;
	}

	Aspect m_aspect;
	EntityManager m_eMgr;
};

struct AlwaysTrueChecker {

	inline bool check( const Entity& e ) {
		return true;
	}
};

/**** Entity Views **************************************************/
// probably pass also EntityFilter <BitChecker>
template <class EntityFilter, class IteratorFactory>
struct View {
	View( EntityManager& emgr )
		: m_eMgr(emgr), m_filter(emgr) {
		// moverlo al entitymanager y dejar alli el override? seria lo suyo
		m_eMgr.tryAddListener<EntityFilter>(m_filter);
	}
	
	// if is_same StorageFilter, remove observer from emgr

	void setBits( const artecshpp::core::ComponentBits& bits ) {
		m_filter.setBits( bits );
		m_itFactory.setBits( bits );
	}
	
	EntityFilter m_filter;
	IteratorFactory m_itFactory;
	BitChecker m_checker;

	template <typename... Args>
	void each(std::function<void(Entity&,Args&...)> f) {
		// ver forma para separar la creación del iterador
		// con la selección de las entidades
		//IteratorType it( = m_filter.begin( );
		// IteratorType es un iterador de entidades
		auto it = m_itFactory.begin( m_filter.entities() );
		while( it != m_itFactory.end( m_filter.entities() ) ) {
			if( m_checker.check(*it) ) {
				f( *it, (m_eMgr.template getComponent<Args>(*it)) ... );
				it++;
			}
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
	virtual void process() = 0;
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
		: m_emgr(emgr), m_view(emgr) {
		m_view.setBits( artecshpp::core::ComponentBitsBuilder<Args...>::buildBits() );
	}
	
	~System() {
		
	}

	void process() override {
		m_view.template each<Args...>(static_cast<Derived*>(this)->function);
	}
	
	void process(Entity e) override {
		m_view.template each<Args...>(e, static_cast<Derived*>(this)->function);
	}
	
private:
	ViewType m_view;	
	EntityManager m_emgr;
	
};

class SampleSystem : public System<
	SampleSystem, 									// system type
	View<AliveFilter, ForwardIteratorFactory>, 		// view type
	int, double, std::string> { 					// needed components
public:
	SampleSystem(EntityManager& emgr)
	: System(emgr) { }
	
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
	
	/****************************/
	Entity e0(0);
	int& e1i = emgr.createComponent<int>(e0);
	double& e1d = emgr.createComponent<double>(e0);
	std::string& e1s = emgr.createComponent<std::string>(e0);
	emgr.addEntity(e0);
	
	/****************************/
	Entity e1(1);
	int& e2i = emgr.createComponent<int>(e1);
	double& e2d = emgr.createComponent<double>(e1);
	std::string& e2s = emgr.createComponent<std::string>(e1);
	emgr.addEntity(e1);
	
	/****************************/
	SampleSystem ss(emgr);
	ss.process(e0);
	
	e1i = 3;
	e1d = 0.5;
	e1s = "olaqase";
	
	e2i = 123;
	e2d = 0.999;
	e2s = "fuckyeah this shit is working";
	
	ss.process(e0);
	
	std::cout << "===============================\n";
	
	ss.process();
	return 0;
}
