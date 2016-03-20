#pragma once

#include <cstdlib>
#include <bitset>

namespace artecshpp {
namespace core {

class Config {
public:
	static const size_t MAX_ENTITIES = 1024;
	static const size_t MAX_COMPONENTS = 64;

};

typedef std::bitset<Config::MAX_COMPONENTS> ComponentBits;

}
}
