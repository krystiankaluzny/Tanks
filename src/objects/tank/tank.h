#ifndef TANK_H
#define TANK_H

#include "../object.h"
#include "bullet.h"

#include <vector>

typedef unsigned TankStateFlags;

class Tank : public Object,  protected InteractiveComponentsHolder
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

        TSF_CREATING = 1 << 7,
        TSF_ALIVE = 1 << 8,

        TSF_PREVIEW = 1 << 9
    };

    Tank(double x, double y, SpriteType type, InteractiveComponents interactive_components);
    virtual ~Tank();

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;
    virtual Bullet *fire();

    Rect nextCollisionRect(Uint32 dt);
    void setDirection(Direction d);
    void collide(Rect &intersect_rect);
    virtual void respawn();
    void creatingState();

    virtual void destroy();
    void setFlag(TankStateFlag flag);
    void clearFlag(TankStateFlag flag);
    bool testFlag(TankStateFlag flag) const;

    Point center() const;
    bool alive() const;
    bool blocked() const;
    Direction movingDirection() const;

    std::vector<Bullet *> bullets;

protected:
    double m_max_speed;
    double m_speed;
    bool m_blocked;

    Direction m_tank_direction;
    Direction m_moving_direction;

    TankStateFlags m_flags;

    unsigned m_lives_count;
    unsigned m_armor_count;

    Uint32 m_slip_time;


    unsigned m_bullet_max_count;

    Object *m_shield;
    Object *m_boat;

    Uint32 m_shield_time;
    Uint32 m_frozen_time;
};

#endif // TANK_H
