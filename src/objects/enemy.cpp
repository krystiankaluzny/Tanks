#include "enemy.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

Enemy::Enemy()
    : Tank()
{
    srand(time(NULL));
    m_direction_time = 0;
    direction = D_DOWN;
    m_speed_time = 0;
    speed = 0;
    m_keep_direction_time = 100;
    m_try_to_go_time = 0;
}

Enemy::Enemy(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
    srand(time(NULL));
    m_direction_time = 0;
    direction = D_DOWN;
    m_speed_time = 0;
    speed = 0;
    m_keep_direction_time = 100;
    m_try_to_go_time = 0;
}

void Enemy::update(Uint32 dt)
{
    Tank::update(dt);

    m_direction_time += dt;
    m_speed_time += dt;
    m_fire_time += dt;
    if(m_direction_time > m_keep_direction_time)
    {
        m_direction_time = 0;
        m_keep_direction_time = rand() % 800 + 100;
        setDirection(static_cast<Direction>(rand() % 4));
    }
    if(m_speed_time > m_try_to_go_time)
    {
        m_speed_time = 0;
        m_try_to_go_time = rand() % 300;
        speed = default_speed;
    }
    if(m_fire_time > m_reload_time)
    {
        m_fire_time = 0;
        m_reload_time = rand() % 1000;
        fire();
    }
}
