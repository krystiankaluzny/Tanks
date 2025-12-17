#include "object.h"
#include <iostream>
#include <algorithm>


Object::Object(double x, double y, SpriteType type)
{
    pos_x = x;
    pos_y = y;
    this->type = type;
    to_erase = false;
    m_sprite = &SpriteConfig::getInstance().getSpriteData(type);
    m_frame_display_time = 0;
    m_current_frame = 0;

    src_rect.x = m_sprite->rect.x;
    src_rect.y = m_sprite->rect.y;
    src_rect.h = m_sprite->rect.h;
    src_rect.w = m_sprite->rect.w;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    collision_rect.x = pos_x;
    collision_rect.y = pos_y;
    collision_rect.h = m_sprite->rect.h;
    collision_rect.w = m_sprite->rect.w;
}

Object::~Object()
{
}

void Object::draw(Renderer &renderer)
{
    if(m_sprite == nullptr || to_erase) return;
    renderer.drawObject(src_rect, dest_rect);
}

void Object::update(Uint32 dt)
{
    if(to_erase) return;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    collision_rect.x = pos_x;
    collision_rect.y = pos_y;
    collision_rect.h = m_sprite->rect.h;
    collision_rect.w = m_sprite->rect.w;

    if(m_sprite->frames_count > 1)
    {
        m_frame_display_time += dt;
        if(m_frame_display_time > m_sprite->frame_duration)
        {
            m_frame_display_time = 0;
            m_current_frame++;
            if(m_current_frame >= m_sprite->frames_count)
            {
                if(m_sprite->loop) m_current_frame = 0;
                else m_current_frame = m_sprite->frames_count - 1;
            }

            src_rect = moveRect(m_sprite->rect, 0, m_current_frame);
        }
    }
}

Rect Object::moveRect(const Rect &rect, int x, int y)
{
    Rect r;
    r.x = rect.x + x*rect.w;
    r.y = rect.y + y*rect.h;
    r.w = rect.w;
    r.h = rect.h;

    return r;
}


Rect intersectRect(Rect *rect1, Rect *rect2)
{
    Rect intersect_rect;
    intersect_rect.x = std::max(rect1->x, rect2->x);
    intersect_rect.y = std::max(rect1->y, rect2->y);
    intersect_rect.w = std::min(rect1->x + rect1->w, rect2->x + rect2->w) - intersect_rect.x;
    intersect_rect.h = std::min(rect1->y + rect1->h, rect2->y + rect2->h) - intersect_rect.y;

    return intersect_rect;
}
