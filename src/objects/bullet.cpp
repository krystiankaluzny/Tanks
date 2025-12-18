#include "bullet.h"
#include "../appconfig.h"
#include "../engine/data/data.h"

Bullet::Bullet(double x, double y)
    : Object(x, y, ST_BULLET)
{
    m_speed = 0.0;
    m_direction = D_UP;
    m_increased_damage = false;
    m_collide = false;
}

Bullet::Bullet(Point tank_center, Size tank_size, Direction direction, double speed)
    : Object(tank_center.x, tank_center.y, ST_BULLET)
{
    m_speed = speed;
    m_direction = direction;
    m_increased_damage = false;
    m_collide = false;

    switch (direction)
    {
    case D_UP:
        pos_x = tank_center.x - dest_rect.w / 2;
        pos_y = tank_center.y - tank_size.h / 2 + 4;
        break;
    case D_RIGHT:
        pos_x = tank_center.x + tank_size.w / 2 - 4;
        pos_y = tank_center.y - dest_rect.h / 2;
        break;
    case D_DOWN:
        pos_x = tank_center.x - dest_rect.w / 2;
        pos_y = tank_center.y + tank_size.h / 2 - 4;
        break;
    case D_LEFT:
        pos_x = tank_center.x - tank_size.w / 2 + 4;
        pos_y = tank_center.y - dest_rect.h / 2;
        break;
    }
}

void Bullet::update(Uint32 dt)
{
    if (!m_collide)
    {
        switch (m_direction)
        {
        case D_UP:
            pos_y -= m_speed * dt;
            break;
        case D_RIGHT:
            pos_x += m_speed * dt;
            break;
        case D_DOWN:
            pos_y += m_speed * dt;
            break;
        case D_LEFT:
            pos_x -= m_speed * dt;
            break;
        }

        src_rect = m_sprite->rect.tiledOffset(m_direction, 0);
        Object::update(dt);
    }
    else
    {
        if (m_sprite->frames_count > 1)
        {
            m_frame_display_time += dt;
            if (m_frame_display_time > m_sprite->frame_duration)
            {
                m_frame_display_time = 0;
                m_current_frame++;
                if (m_current_frame >= m_sprite->frames_count)
                    to_erase = true;

                src_rect = m_sprite->rect.tiledOffset(0, m_current_frame);
            }
        }
    }
}

void Bullet::increaseSpeed(double speed_factor)
{
    m_speed *= speed_factor;
}

void Bullet::increaseDamage()
{
    m_increased_damage = true;
}

void Bullet::destroy()
{
    if (m_collide)
        return;

    m_collide = true;
    m_speed = 0;
    m_current_frame = 0;
    m_frame_display_time = 0;
    m_sprite = &SpriteConfig::getInstance().getSpriteData(ST_DESTROY_BULLET);

    switch (m_direction)
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

    collision_rect = {0, 0, 0, 0};
}

bool Bullet::isDamageIncreased() const
{
    return m_increased_damage;
}

bool Bullet::isColide() const
{
    return m_collide;
}

Direction Bullet::direction() const
{
    return m_direction;
}