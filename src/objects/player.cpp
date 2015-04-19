#include "player.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player()
    : Tank()
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

    if (!key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_LEFT] && !key_state[SDL_SCANCODE_DOWN] && !key_state[SDL_SCANCODE_RIGHT])
    {
        if(!testFlag(TSF_ON_ICE) || m_slip_time == 0)
            speed = 0.0;
    }

    Tank::update(dt);
}

void Player::respawn()
{
    m_lives_count--;
    if(m_lives_count <= 0)
    {
        if(bullet == nullptr) to_erase = true;
        return;
    }

    pos_x = AppConfig::player1_starting_point.x;
    pos_y= AppConfig::player1_starting_point.y;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    Tank::respawn();
}
