#include <iostream>


#include "artecshpp/core/aspect.h"
#include "artecshpp/core/component.h"
#include "artecshpp/core/engine.h"
#include "artecshpp/core/entity.h"
#include "artecshpp/core/entitysystem.h"

class A {};
class B {};
class C {};
class D {};
class E {};
class F {};
class G {};
class H {};

int main( int argc, char** argv )
{
	std::cout << "ARTECSHPP" << std::endl;
	artecshpp::core::Aspect a1, a2, a3, a4;
	a1.all<A,B,C,D,E,F,G,H>();
	a2.all<A,C,E,G>();
	a3.all<B,E,H>();
	a4.all<H,G,A>();
	std::cout << a1.all() << std::endl;
	std::cout << a2.all() << std::endl;
	std::cout << a3.all() << std::endl;
	std::cout << a4.all() << std::endl;
	return 0;
}
