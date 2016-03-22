#include <functional>
#include <iterator>
#include <iostream>

#include <artecshpp/core/entity.h>
#include <artecshpp/core/system.h>

typedef artecshpp::Entity Entity;
typedef artecshpp::Aspect Aspect;
typedef artecshpp::EntityManager EntityManager;

/**** EntityManager ***************************************/
/**** Iterator Factories ******************************************/


class SampleSystem : public artecshpp::System<
	SampleSystem, 									// system type
	artecshpp::View<artecshpp::AliveFilter, artecshpp::ForwardIteratorFactory>, 		// view type
	int, double, std::string> { 					// needed components
public:
	SampleSystem(EntityManager& emgr)
	: System(emgr) { }
	
	std::function<void(Entity e, int& i, double& d, std::string& str)> function =
		[](Entity e, int& i, double& d, std::string& s) {
			std::cout 	<< i << std::endl
						<< d << std::endl
						<< s << std::endl;
		};
};

class SampleSystem2 : public artecshpp::System<
	SampleSystem2, 									// system type
	artecshpp::View<artecshpp::AliveFilter, artecshpp::CheckerIteratorFactory>, 		// view type
	float> { 					// needed components
public:
	SampleSystem2(EntityManager& emgr)
	: System(emgr) { }
	
	std::function<void(Entity, float&)> function =
		[](Entity e, float& f) {
			std::cout << f << std::endl;
		};
};

class SampleSystem3 : public artecshpp::System<
	SampleSystem3, 									// system type
	artecshpp::View<artecshpp::StorageFilter, artecshpp::ForwardIteratorFactory>, 		// view type
	float> { 					// needed components
public:
	SampleSystem3(EntityManager& emgr)
	: System(emgr) { }
	
	std::function<void(Entity, float&)> function =
		[](Entity e, float& f) {
			std::cout << f << std::endl;
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

	Entity e0 = emgr.createEntity();

	int& e1i = emgr.createComponent<int>(e0);
	double& e1d = emgr.createComponent<double>(e0);
	std::string& e1s = emgr.createComponent<std::string>(e0);
	float& e1f = emgr.createComponent<float>(e0);
	e1f = 3.14159268;
	emgr.addEntity(e0);

	Entity e1 = emgr.createEntity();
	int& e2i = emgr.createComponent<int>(e1);
	double& e2d = emgr.createComponent<double>(e1);
	std::string& e2s = emgr.createComponent<std::string>(e1);
	emgr.addEntity(e1);
	
	std::cout << emgr.getBits(e0) << " " << emgr.getBits(e1) << "\n";
	
	SampleSystem ss(emgr);
	SampleSystem2 ss2(emgr);
	SampleSystem3 ss3(emgr);
	
	std::cout << "num observers: " << " " << emgr.numObservers() << std::endl;
	ss.process(e0);

	e1i = 3;
	e1d = 0.5;
	e1s = "heh";
	e2i = 123;
	e2d = 0.999;
	e2s = "working!";
	
	ss.process(e0);
	
	std::cout << "===============================\n";
	
	ss.process();

	std::cout << "===============================\n";

	ss2.process();
	//artecshpp::Aspect aspect;
	//CheckerIteratorFactory cif(emgr, aspect);

	return 0;
}
