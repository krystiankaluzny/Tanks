#include "bonus.h"
#include "../appconfig.h"

Bonus::Bonus()
    : Object(0, 0, ST_BONUS_STAR)
{
    m_bonus_show_time = 0;
    m_show = true;
}

Bonus::Bonus(double x, double y, SpriteType type)
    : Object(x, y, type)
{
    m_bonus_show_time = 0;
    m_show = true;
}

void Bonus::draw()
{
    if(m_show) Object::draw();
}

void Bonus::update(Uint32 dt)
{
    Object::update(dt);
    m_bonus_show_time += dt;
    if(m_bonus_show_time > AppConfig::bonus_show_time)
        to_erase = true;

    if(m_bonus_show_time / (m_bonus_show_time < AppConfig::bonus_show_time / 4 * 3 ? AppConfig::bonus_blink_time : AppConfig::bonus_blink_time / 2) % 2)
        m_show = true;
    else m_show = false;
}
