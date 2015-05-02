#include "bonus.h"
#include "../appconfig.h"

Bonus::Bonus()
    : Object(0, 0, ST_BRICK_WALL)
{
    m_bonus_show_time = 0;
}

Bonus::Bonus(double x, double y, SpriteType type)
    : Object(x, y, type)
{
    m_bonus_show_time = 0;
}

void Bonus::update(Uint32 dt)
{
    m_bonus_show_time += dt;
    if(m_bonus_show_time > AppConfig::bonus_show_time)
        to_erase = true;
}
