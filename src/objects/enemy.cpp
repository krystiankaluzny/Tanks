#include "enemy.h"
#include "../appconfig.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>

Enemy::Enemy(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
    direction = D_DOWN;
    m_direction_time = 0;
    m_keep_direction_time = 100;

    m_speed_time = 0;
    m_try_to_go_time = 100;

    m_fire_time = 0;
    m_reload_time = 100;
    lives_count = 1;

    m_bullet_max_size = 1;

    m_frozen_time = 0;

    if (type == ST_TANK_B)
        default_speed = AppConfig::tank_default_speed * 1.3;
    else
        default_speed = AppConfig::tank_default_speed;

    target_position = {-1, -1};

    respawn();
}

void Enemy::draw(Renderer &renderer)
{
    if (to_erase)
        return;
    Tank::draw(renderer);
}

// TODO coment more
void Enemy::update(Uint32 dt)
{
    if (to_erase)
        return;
    Tank::update(dt);

    if (testFlag(TSF_ALIVE))
    {
        if (testFlag(TSF_BONUS))
            src_rect = m_sprite->rect.tiledOffset((testFlag(TSF_ON_ICE) ? new_direction : direction) - 4, m_current_frame);
        else
            src_rect = m_sprite->rect.tiledOffset((testFlag(TSF_ON_ICE) ? new_direction : direction) + (lives_count - 1) * 4, m_current_frame);
    }
    else
        src_rect = m_sprite->rect.tiledOffset(0, m_current_frame);

    if (testFlag(TSF_FROZEN))
        return;

    m_direction_time += dt;
    m_speed_time += dt;
    m_fire_time += dt;
    if (m_direction_time > m_keep_direction_time)
    {
        m_direction_time = 0;
        m_keep_direction_time = rand() % 800 + 100;

        float p = static_cast<float>(rand()) / RAND_MAX;

        if (p < (type == ST_TANK_A ? 0.8 : 0.5) && target_position.x > 0 && target_position.y > 0)
        {
            int dx = target_position.x - (dest_rect.x + dest_rect.w / 2);
            int dy = target_position.y - (dest_rect.y + dest_rect.h / 2);

            p = static_cast<float>(rand()) / RAND_MAX;

            if (abs(dx) > abs(dy))
                setDirection(p < 0.7 ? (dx < 0 ? D_LEFT : D_RIGHT) : (dy < 0 ? D_UP : D_DOWN));
            else
                setDirection(p < 0.7 ? (dy < 0 ? D_UP : D_DOWN) : (dx < 0 ? D_LEFT : D_RIGHT));
        }
        else
            setDirection(static_cast<Direction>(rand() % 4));
    }
    if (m_speed_time > m_try_to_go_time)
    {
        m_speed_time = 0;
        m_try_to_go_time = rand() % 300;
        speed = default_speed;
    }
    if (m_fire_time > m_reload_time)
    {
        m_fire_time = 0;
        if (type == ST_TANK_D)
        {
            m_reload_time = rand() % 400;
            int dx = target_position.x - (dest_rect.x + dest_rect.w / 2);
            int dy = target_position.y - (dest_rect.y + dest_rect.h / 2);

            if (stop)
                fire();
            else
                switch (direction)
                {
                case D_UP:
                    if (dy < 0 && abs(dx) < dest_rect.w)
                        fire();
                    break;
                case D_RIGHT:
                    if (dx > 0 && abs(dy) < dest_rect.h)
                        fire();
                    break;
                case D_DOWN:
                    if (dy > 0 && abs(dx) < dest_rect.w)
                        fire();
                    break;
                case D_LEFT:
                    if (dx < 0 && abs(dy) < dest_rect.h)
                        fire();
                    break;
                }
        }
        else if (type == ST_TANK_C)
        {
            m_reload_time = rand() % 800;
            fire();
        }
        else
        {
            m_reload_time = rand() % 1000;
            fire();
        }
    }

    stop = false;
}

Bullet *Enemy::fire()
{
    Bullet *b = Tank::fire();

    if (b != nullptr && type == ST_TANK_C)
    {
        b->increaseSpeed(1.3);
    }

    return b;
}
void Enemy::hit()
{
    if (lives_count == 1)
    {
        lives_count = 0;
        Tank::destroy();
    }
    else if (lives_count > 1)
    {
        //    clearFlag(TSF_BONUS); //możliwe jednokrotne wypadnięcie bonusu
        lives_count--;
    }
}

unsigned Enemy::scoreForHit()
{
    if (lives_count > 0)
        return 50;
    return 100;
}
