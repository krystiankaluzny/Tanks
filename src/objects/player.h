#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"

class Player : public Tank
{
public:
    struct PlayerKeys
    {
        PlayerKeys(): up(SDL_SCANCODE_UNKNOWN), down(SDL_SCANCODE_UNKNOWN), left(SDL_SCANCODE_UNKNOWN), right(SDL_SCANCODE_UNKNOWN), fire(SDL_SCANCODE_UNKNOWN) {}
        PlayerKeys(SDL_Scancode u, SDL_Scancode d, SDL_Scancode l, SDL_Scancode r, SDL_Scancode f): up(u), down(d), left(l), right(r), fire(f) {}
        SDL_Scancode up;
        SDL_Scancode down;
        SDL_Scancode left;
        SDL_Scancode right;
        SDL_Scancode fire;
    };

    Player();
    Player(double x, double y, SpriteType type);

    void update(Uint32 dt);
    void respawn();
    void destroy();
    Bullet* fire();
    void changeStarCountBy(int c);

    PlayerKeys player_keys;
    unsigned score;
    int star_count;

private:
    Uint32 m_fire_time;
};

#endif // PLAYER_H
