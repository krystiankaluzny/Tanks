#ifndef BRICK_H
#define BRICK_H
#include "object.h"

class Brick : public Object
{
public:
    Brick();
    Brick(double x, double y);

    void update(Uint32 dt);
    void bulletHit(Direction bullet_direction);
private:
    int m_colision_count;
    int m_state_code;
};

#endif // BRICK_H
