#include "player.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player()
    : Tank(0, 0, ST_PLAYER_1)
{
    speed = 0;
    m_lives_count = 4;
    respawn();
}

Player::Player(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
   speed = 0;
   m_lives_count = 4;
   respawn();
}

void Player::update(Uint32 dt)
{
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (!key_state[player_keys.up] && !key_state[player_keys.down] && !key_state[player_keys.left] && !key_state[player_keys.right])
    {
        if(!testFlag(TSF_ON_ICE) || m_slip_time == 0)
            speed = 0.0;
    }

    Tank::update(dt);

    if(testFlag(TSF_LIFE))
        src_rect = moveRect(m_sprite->rect, (testFlag(TSF_ON_ICE) ? new_direction : direction), m_current_frame);
    else
        src_rect = moveRect(m_sprite->rect, 0, m_current_frame);

}

void Player::respawn()
{
    m_lives_count--;
    if(m_lives_count <= 0)
    {
        if(bullet == nullptr) to_erase = true;
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

    Tank::respawn();
}
