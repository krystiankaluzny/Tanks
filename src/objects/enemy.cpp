#include "enemy.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

Enemy::Enemy()
    : Tank()
{
    srand(time(NULL));
    change_direction_time = 0;
    direction = D_DOWN;
    speed_time = 0;
    speed = 0;
}

Enemy::Enemy(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
    srand(time(NULL));
    change_direction_time = 0;
    direction = D_DOWN;
    speed_time = 0;
    speed = 0;
}

void Enemy::update(Uint32 dt)
{
    Tank::update(dt);

    change_direction_time += dt;
    speed_time += dt;
    if(change_direction_time > 500)
    {
        change_direction_time = 0;
            std::cout  << "DUAP" << std::endl;
        setDirection(static_cast<Direction>(rand() % 4));
    }
    if(speed_time > 150)
    {
        speed_time = 0;
        speed = default_speed;
    }
    fire();
}
