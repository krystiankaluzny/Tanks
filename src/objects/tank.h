#ifndef TANK_H
#define TANK_H

#include "object.h"
#include "bullet.h"
#include "../type.h"
#include "objectfactory.h"

typedef unsigned TankStateFlags;

class Tank : public Object
{
public:
    Tank();
    Tank(double x, double y, SpriteType type);
    virtual ~Tank();

    void draw();
    void update(Uint32 dt);
    void fire();
    SDL_Rect nextCollisionRect(Uint32 dt);
    void setDirection(Direction d);
    void collide(SDL_Rect& intersect_rect);
    virtual void respawn();
    virtual void destroy();
    void setFlag(TankStateFlag flag);
    void clearFlag(TankStateFlag flag);
    bool testFlag(TankStateFlag flag);

    double speed;
    bool stop;
    static constexpr double default_speed = 0.08;
    Direction direction;
    Bullet* bullet;

protected:

    TankStateFlags m_flags;
    Sint32 m_slip_time;
    Direction new_direction;


    //1 star tank speed v = 0.084
    //bullet
    //normaln bullet speed v = 0.23;
    //1 star bullet speed v = 0.42;

};

#endif // TANK_H
