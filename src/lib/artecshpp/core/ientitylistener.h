#ifndef IENTITYLISTENER_H
#define IENTITYLISTENER_H

#include "entity.h"

namespace artecshpp {
namespace core {


class IEntityListener
{
public:
    virtual ~IEntityListener() = 0 ;

	virtual void entityAdded( Entity* e ) = 0 ;
	virtual void entityRemoved( Entity* e ) = 0 ;

};


}}

#endif // IENTITYLISTENER_H
