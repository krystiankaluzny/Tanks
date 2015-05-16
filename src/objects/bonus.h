#ifndef BONUS_H
#define BONUS_H

#include "object.h"

class Bonus : public Object
{
public:
    /**
     * Tworzenie domyślnego bonusu - gwiazdki w położeniu (0, 0).
     */
    Bonus();
    /**
     * Tworzenie bonusu.
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ bonusu
     */
    Bonus(double x, double y, SpriteType type);

    void draw();
    void update(Uint32 dt);
private:
    Uint32 m_bonus_show_time;
    bool m_show;
};

#endif // BONUS_H
