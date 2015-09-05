#include <stdlib.h>

#include "aspect.h"


namespace artecshpp {
namespace core {


Aspect::~Aspect ()
{

}

template<typename... Args>
void Aspect::all()
{
	m_all |= ComponentBitsBuilder<Args...>::buildBits();
}

const ComponentBits& Aspect::all()
{
	return m_all;
}


}}


