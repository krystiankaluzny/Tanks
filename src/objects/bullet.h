#ifndef BULLET_H
#define BULLET_H

#include "object.h"

class Bullet : public Object
{
public:
    /**
     * Tworzenie pocisku w położeniu (0, 0).
     */
    Bullet();
    /**
     * Tworzenie pocisku
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     */
    Bullet(double x, double y);

    void update(Uint32 dt);
    void destroy();
    
    double speed;
    bool collide;
    bool increased_damage;
    Direction direction;
};

#endif // BULLET_H
