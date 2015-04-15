#ifndef ENEMY_H
#define ENEMY_H
#include "tank.h"

class Enemy : public Tank
{
public:
    Enemy();
    Enemy(double x, double y, SpriteType type);

    void update(Uint32 dt);

private:
    Uint32 change_direction_time;
    Uint32 speed_time;
};

#endif // ENEMY_H
