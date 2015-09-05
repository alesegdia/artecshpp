#include <stdlib.h>

#include "aspect.h"


namespace artecshpp {
namespace core {


Aspect::~Aspect ()
{

}

const ComponentBits& Aspect::all()
{
	return m_all;
}


}}


