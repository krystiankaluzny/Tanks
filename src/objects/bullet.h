#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include "direction.h"


class Bullet : public Object
{
public:

    Bullet(double x, double y);

    void update(Uint32 dt) override;
    void destroy();
    

    //TODO make it private
    double speed;
    bool collide;
    bool increased_damage;
    Direction direction;
};

#endif // BULLET_H
