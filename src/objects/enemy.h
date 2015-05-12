#ifndef ENEMY_H
#define ENEMY_H

#include "tank.h"

class Enemy : public Tank
{
public:
    Enemy();
    Enemy(double x, double y, SpriteType type);

    void draw();
    void update(Uint32 dt);
    void destroy();
    unsigned scoreForHit();

    SDL_Point target_position;

private:
    Uint32 m_direction_time;
    Uint32 m_keep_direction_time;

    Uint32 m_speed_time;
    Uint32 m_try_to_go_time;

    Uint32 m_fire_time;
    Uint32 m_reload_time;
};

#endif // ENEMY_H
