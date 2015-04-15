#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "object.h"
#include "../type.h"

class ObjectFactory
{
public:
    static Object* Create(int x, int y, SpriteType sp);
};

#endif // OBJECTFACTORY_H
