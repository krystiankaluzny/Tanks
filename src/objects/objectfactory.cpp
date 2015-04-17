#include "objectfactory.h"
#include "brick.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

Object *ObjectFactory::Create(int x, int y, SpriteType sp)
{
    switch(sp)
    {
    case ST_BRICK_WALL:
        return new Brick(x, y);
    case ST_STONE_WALL:
    case ST_WATER:
    case ST_BUSH:
    case ST_ICE:
        return new Object(x, y, sp);
    case ST_BULLET:
        return new Bullet(x, y);

    default:
        return nullptr;
    }
}
