#include "player.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player()
    : Tank(AppConfig::player_starting_point.at(0).x, AppConfig::player_starting_point.at(0).y, ST_PLAYER_1)
{
    speed = 0;
    lives_count = 11;
    m_bullet_max_size = AppConfig::player_bullet_max_size;
    score = 0;
    star_count = 0;
    m_shield = new Object(0, 0, ST_SHIELD);
    m_shield_time = 0;
    respawn();
}

Player::Player(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
   speed = 0;
   lives_count = 11;
   m_bullet_max_size = AppConfig::player_bullet_max_size;
   score = 0;
   star_count = 0;
   m_shield = new Object(x, y, ST_SHIELD);
   m_shield_time = 0;
   respawn();
}

void Player::update(Uint32 dt)
{
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    Tank::update(dt);

    if(key_state != nullptr && !testFlag(TSF_MENU))
    {
        if(key_state[player_keys.up])
        {
            setDirection(D_UP);
            speed = default_speed;
        }
        else if(key_state[player_keys.down])
        {
            setDirection(D_DOWN);
            speed = default_speed;
        }
        else if(key_state[player_keys.left])
        {
            setDirection(D_LEFT);
            speed = default_speed;
        }
        else if(key_state[player_keys.right])
        {
            setDirection(D_RIGHT);
            speed = default_speed;
        }
        else
        {
            if(!testFlag(TSF_ON_ICE) || m_slip_time == 0)
                speed = 0.0;
        }

        if(key_state[player_keys.fire] && m_fire_time > AppConfig::player_reload_time)
        {
            fire();
            m_fire_time = 0;
        }
    }

    m_fire_time += dt;

    if(testFlag(TSF_LIFE))
        src_rect = moveRect(m_sprite->rect, (testFlag(TSF_ON_ICE) ? new_direction : direction), m_current_frame + 2 * star_count);
    else
        src_rect = moveRect(m_sprite->rect, 0, m_current_frame + 2 * star_count);

    stop = false;
}

void Player::respawn()
{
    lives_count--;
    if(lives_count <= 0)
    {
        if(bullets.size() == 0) to_erase = true;
        return;
    }

    if(type == ST_PLAYER_1)
    {
        pos_x = AppConfig::player_starting_point.at(0).x;
        pos_y = AppConfig::player_starting_point.at(0).y;
    }
    else
    {
        pos_x = AppConfig::player_starting_point.at(1).x;
        pos_y = AppConfig::player_starting_point.at(1).y;
    }

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    setDirection(D_UP);
    Tank::respawn();
    setFlag(TSF_SHIELD);
    m_shield_time = AppConfig::tank_shield_time / 2;
}

void Player::destroy()
{
    if(testFlag(TSF_SHIELD)) return;
    if(testFlag(TSF_BOAT))
    {
        clearFlag(TSF_BOAT);
        return;
    }

    if(star_count == 3)
        changeStarCountBy(-1);
    else
    {
        changeStarCountBy(-3);
        Tank::destroy();
    }
}

Bullet* Player::fire()
{
    Bullet* b = Tank::fire();
    if(b != nullptr)
    {
        if(star_count > 0) b->speed = AppConfig::bullet_default_speed * 1.3;
        if(star_count == 3) b->increased_damage = true;
    }
    return b;
}

void Player::changeStarCountBy(int c)
{
    star_count += c;
    if(star_count > 3) star_count = 3;
    else if(star_count < 0) star_count = 0;

    if(star_count >= 2 && c > 0) m_bullet_max_size++;
    else m_bullet_max_size = 2;

    if(star_count > 0) default_speed = AppConfig::tank_default_speed * 1.3;
    else default_speed = AppConfig::tank_default_speed;
}
