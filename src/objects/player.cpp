#include "player.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player()
    : Tank()
{
    speed = 0;
    respawn();
}

Player::Player(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
   speed = 0;
   respawn();
}

void Player::update(Uint32 dt)
{
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (!key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_LEFT] && !key_state[SDL_SCANCODE_DOWN] && !key_state[SDL_SCANCODE_RIGHT])
        speed = 0.0;

    Tank::update(dt);
}

void Player::respawn()
{
    pos_x = AppConfig::player1_starting_point_x;
    pos_y= AppConfig::player1_starting_point_y;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    Tank::respawn();
}
