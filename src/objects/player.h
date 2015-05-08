#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"

class Player : public Tank
{
public:
    struct PlayerKeys
    {
        PlayerKeys(): up(SDL_SCANCODE_UP), down(SDL_SCANCODE_DOWN), left(SDL_SCANCODE_LEFT), right(SDL_SCANCODE_RIGHT), fire(SDL_SCANCODE_SPACE) {}
        PlayerKeys(SDL_Scancode u, SDL_Scancode d, SDL_Scancode l, SDL_Scancode r, SDL_Scancode f): up(u), down(d), left(l), right(r), fire(f) {}
        SDL_Scancode up;
        SDL_Scancode down;
        SDL_Scancode left;
        SDL_Scancode right;
        SDL_Scancode fire;
    };

    Player();
    Player(double x, double y, SpriteType type);
    ~Player();

    void draw();
    void update(Uint32 dt);
    void respawn();
    void destroy();

    PlayerKeys player_keys;
    unsigned score;
    unsigned star_count;
private:
    Object* m_shield;
    SpriteData* m_shield_sprite;
    Uint32 m_shield_time;
    Uint32 m_shield_frame_time;
    int m_shield_frame;
};

#endif // PLAYER_H
