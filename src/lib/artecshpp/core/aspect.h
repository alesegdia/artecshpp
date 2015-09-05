#ifndef __ASPECT_H__
#define __ASPECT_H__

#include "config.h"
#include "componenttraits.h"


namespace artecshpp {
namespace core {


class Aspect {
public:
	virtual ~Aspect ();

	template<typename... Args>
	void all()
	{
		m_all |= ComponentBitsBuilder<Args...>::buildBits();
	}

	const ComponentBits& all();


private:
	ComponentBits m_all;


};



}}


#endif

