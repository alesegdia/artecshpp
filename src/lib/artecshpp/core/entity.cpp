#include <stdlib.h>

#include "entity.h"
#include "componenttraits.h"

namespace artecshpp {


IEntityListener::~IEntityListener()
{
}

Entity::eid_t EntityManager::s_lastID = 0;


}


