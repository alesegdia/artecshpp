#ifndef IENTITYLISTENER_H
#define IENTITYLISTENER_H

#include "entity.h"

namespace artecshpp {
namespace core {


class IEntityListener
{
public:
	IEntityListener();
	~IEntityListener();

	void entityAdded( Entity e );
	void entityRemoved( Entity e );

};


}}

#endif // IENTITYLISTENER_H
