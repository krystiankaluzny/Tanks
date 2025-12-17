#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include "direction.h"


class Bullet : public Object
{
public:
    Bullet(double x, double y);
    Bullet(Point tank_center, Size tank_size, Direction direction, double speed);

    void update(Uint32 dt) override;

    void increaseSpeed(double speed_factor);
    void increaseDamage();
    void destroy();

    bool isDamageIncreased() const;
    bool isColide() const;
    Direction direction() const;
    

private:
    double m_speed;
    bool m_collide;
    bool m_increased_damage;
    Direction m_direction;
};

#endif // BULLET_H
