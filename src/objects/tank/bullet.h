#ifndef TANK_BULLET_H
#define TANK_BULLET_H

#include "../object.h"
#include "direction.h"

class Tank;

class Bullet : public Object
{
public:
    Bullet(double x, double y);
    Bullet(Tank* owner, Direction direction, double speed);

    void update(Uint32 dt) override;

    void increaseSpeed(double speed_factor);
    void increaseDamage();
    void destroy();

    bool isDamageIncreased() const;
    bool isColide() const;
    Direction direction() const;
    Tank* owner() const;

private:
    Tank* m_owner;
    double m_speed;
    bool m_collide;
    bool m_increased_damage;
    Direction m_direction;
};

#endif // TANK_BULLET_H
