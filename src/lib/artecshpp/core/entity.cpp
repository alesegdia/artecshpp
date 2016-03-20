#include <stdlib.h>

#include "entity.h"
#include "componenttraits.h"

namespace artecshpp {
namespace core {


IEntityListener::~IEntityListener()
{
}

Entity::eid_t EntityManager::s_lastID = 0;


}}


