#pragma once

#include "config.h"
#include "componenttraits.h"


namespace artecshpp {


class Aspect {
public:
	virtual ~Aspect ();

	template<typename... Args>
	void all()
	{
		m_all |= ComponentBitsBuilder<Args...>::buildBits();
	}

	const ComponentBits& all();

	bool fits(ComponentBits cbits)
	{
		return true;
	}


private:
	ComponentBits m_all;


};



}

