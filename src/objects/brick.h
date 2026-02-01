#ifndef BRICK_H
#define BRICK_H

#include "object.h"
#include "tank/direction.h"

/**
 * @brief Klasa odpowiedzialna za pojedyńczy kawałek murku.
 */
class Brick : public Object
{
public:
    Brick(double x, double y);

    void update(Uint32 dt) override;
    void bulletHit(Direction bullet_direction);
private:
    int m_bullet_hit_count;
    int m_state_code;
};

#endif // BRICK_H
