#ifndef BRICK_H
#define BRICK_H

#include "object.h"

class Brick : public Object
{
public:
    /**
     * Tworzenie murku w położeniu (0, 0).
     */
    Brick();
    /**
     * Tworzenie murku
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     */
    Brick(double x, double y);

    void update(Uint32 dt);
    void bulletHit(Direction bullet_direction);
private:
    int m_collision_count;
    int m_state_code;
};

#endif // BRICK_H
