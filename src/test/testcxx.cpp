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


/**************************************************************************/
template <typename T>
struct M2 {
	std::string str() {
		std::stringstream ss;
		ss << "SpecMapper<" << GET<T>::getObj() << ">";
		return ss.str();
	}
};

/* *****************************************************/


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

/*************************************************************************/

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
