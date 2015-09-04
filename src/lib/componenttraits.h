#ifndef COMPONENTTRAITS_H
#define COMPONENTTRAITS_H

#include <cstdint>
#include <cmath>


namespace artecshpp {

	
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


}


#endif // COMPONENTTRAITS_H
