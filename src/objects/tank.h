#ifndef TANK_H
#define TANK_H

#include "object.h"
#include "bullet.h"

#include <vector>

typedef unsigned TankStateFlags;


class Tank : public Object
{
public:
    enum TankStateFlag
    {
        TSF_SHIELD = 1 << 1,
        TSF_FROZEN = 1 << 2,
        TSF_DESTROYED = 1 << 3,
        TSF_BOAT = 1 << 4,
        TSF_BONUS = 1 << 5,
        TSF_ON_ICE = 1 << 6,
        TSF_CREATE = 1 << 7,
        TSF_LIFE = 1 << 8,
        TSF_MENU = 1 << 9 // TODO rename 
    };


    Tank(double x, double y, SpriteType type);
    virtual ~Tank();

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;
    virtual Bullet *fire();

    Rect nextCollisionRect(Uint32 dt);
    void setDirection(Direction d);
    void collide(Rect &intersect_rect);
    virtual void respawn();

    virtual void destroy();
    void setFlag(TankStateFlag flag);
    void clearFlag(TankStateFlag flag);
    bool testFlag(TankStateFlag flag);

    Point getCenter() const;

    //TODO make it private or protected
    double default_speed;
    double speed;
    bool stop;
    Direction direction;
    std::vector<Bullet *> bullets;
    int lives_count;

protected:
    TankStateFlags m_flags;

    //TODO change to Uint32
    Sint32 m_slip_time;
    Direction new_direction;

    //TODO rename to _count
    unsigned m_bullet_max_size;


    Object *m_shield;
    Object *m_boat;

    Uint32 m_shield_time;
    Uint32 m_frozen_time;
};

#endif // TANK_H
