#include "eagle.h"
#include "../engine/data/data.h"


Eagle::Eagle(double x, double y)
    : Object(x, y, ST_EAGLE)
{
}

void Eagle::update(Uint32 dt)
{
    if(type == ST_DESTROY_EAGLE)
    {
        m_frame_display_time += dt;
        if(m_frame_display_time > m_sprite->frame_duration)
        {
            m_frame_display_time = 0;
            m_current_frame++;
            if(m_current_frame >= m_sprite->frames_count)
            {
                m_sprite = &SpriteConfig::getInstance().getSpriteData(ST_FLAG);
                m_current_frame = 0;
                type = ST_FLAG;
                update(0);
            }
            src_rect = m_sprite->rect.tiledOffset(0, m_current_frame);
        }
    }
    else
        Object::update(dt);
}

void Eagle::destroy()
{
    if(type != ST_EAGLE) return;

    m_sprite = &SpriteConfig::getInstance().getSpriteData(ST_DESTROY_EAGLE);
    type = ST_DESTROY_EAGLE;
    src_rect = m_sprite->rect.tiledOffset(0, 0);

    dest_rect.x = pos_x + (dest_rect.w - m_sprite->rect.w)/2;
    dest_rect.y = pos_y + (dest_rect.h - m_sprite->rect.h)/2;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;
}
