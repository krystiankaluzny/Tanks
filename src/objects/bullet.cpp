#include "bullet.h"
#include "../appconfig.h"

Bullet::Bullet()
    : Object(0, 0, ST_BULLET)
{
    speed = 0.0;
    direction = D_UP;
    increased_damage = false;
    collide = false;
}

Bullet::Bullet(double x, double y)
    : Object(x, y, ST_BULLET)
{
    speed = 0.0;
    direction = D_UP;
    increased_damage = false;
    collide = false;
}

void Bullet::update(Uint32 dt)
{
    if(!collide)
    {
        switch (direction)
        {
        case D_UP:
            pos_y -= speed * dt;
            break;
        case D_RIGHT:
            pos_x += speed * dt;
            break;
        case D_DOWN:
            pos_y += speed * dt;
            break;
        case D_LEFT:
            pos_x -= speed * dt;
            break;
        }

        src_rect = moveRect(m_sprite->rect, direction, 0);
        Object::update(dt);
    }
    else
    {
        if(m_sprite->frames_count > 1)
        {
            m_frame_display_time += dt;
            if(m_frame_display_time > m_sprite->frame_duration)
            {
                m_frame_display_time = 0;
                m_current_frame++;
                if(m_current_frame >= m_sprite->frames_count)
                    to_erase = true;

                src_rect = moveRect(m_sprite->rect, 0, m_current_frame);
            }
        }
    }
}

void Bullet::destroy()
{
    if(collide) return; //zapogiega wielogrotnmu wywołaniu

    collide = true;
    speed = 0;
    m_current_frame = 0;
    m_frame_display_time = 0;
    m_sprite = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_DESTROY_BULLET);

    switch(direction)
    {
    case D_UP:
        dest_rect.x = pos_x + (dest_rect.w - m_sprite->rect.w) / 2; // dest_rect.w, dest_rect.h - stary rozmiar pocisku
        dest_rect.y = pos_y - m_sprite->rect.h / 2;
        break;
    case D_RIGHT:
        dest_rect.x = pos_x + dest_rect.w - m_sprite->rect.w / 2;
        dest_rect.y = pos_y + (dest_rect.h - m_sprite->rect.h) / 2;
        break;
    case D_DOWN:
        dest_rect.x = pos_x + (dest_rect.w - m_sprite->rect.w) / 2;
        dest_rect.y = pos_y + dest_rect.h - m_sprite->rect.h / 2;
        break;
    case D_LEFT:
        dest_rect.x = pos_x - m_sprite->rect.w / 2;
        dest_rect.y = pos_y + (dest_rect.h - m_sprite->rect.h) / 2;
        break;
    }

    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    src_rect.x = m_sprite->rect.x;
    src_rect.y = m_sprite->rect.y;
    src_rect.h = m_sprite->rect.h;
    src_rect.w = m_sprite->rect.w;

}
