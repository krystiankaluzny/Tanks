#include "enemy.h"
#include "../appconfig.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

Enemy::Enemy()
    : Tank()
{
    srand(time(NULL));
    direction = D_DOWN;
    m_direction_time = 0;
    m_speed_time = 0;
    m_keep_direction_time = 100;
    m_try_to_go_time = 0;
    m_lives_count = 1;

    if(type == ST_TANK_B)
        default_speed = AppConfig::tank_default_speed * 1.3;
    else
        default_speed = AppConfig::tank_default_speed;

    respawn();
}

Enemy::Enemy(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
    srand(time(NULL));
    direction = D_DOWN;
    m_direction_time = 0;
    m_speed_time = 0;
    m_keep_direction_time = 100;
    m_try_to_go_time = 0;
    m_lives_count = 1;

    if(type == ST_TANK_B)
        default_speed = AppConfig::tank_default_speed * 1.3;
    else
        default_speed = AppConfig::tank_default_speed;

    respawn();
}

void Enemy::draw()
{

    Tank::draw();
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

void Enemy::destroy()
{
    m_lives_count--;
    if(m_lives_count <= 0)
        Tank::destroy();
}
