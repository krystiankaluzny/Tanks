#ifndef BONUS_H
#define BONUS_H

#include "object.h"

class Bonus : public Object
{
public:
    Bonus();
    Bonus(double x, double y, SpriteType type);

    void draw();
    void update(Uint32 dt);
private:
    Uint32 m_bonus_show_time;
    bool m_show;
};

#endif // BONUS_H
