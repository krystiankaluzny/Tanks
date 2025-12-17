#include "brick.h"
#include <iostream>
#include "../engine/data/data.h"


Brick::Brick(double x, double y)
    : Object(x, y, ST_BRICK_WALL)
{
    m_bullet_hit_count = 0;
    m_state_code = 0;
}

void Brick::update(Uint32 dt)
{
    //do nothing (don't change collision_rect)
}

void Brick::bulletHit(Direction bullet_direction)
{
    int bd = bullet_direction;
    m_bullet_hit_count++;
    if(m_bullet_hit_count == 1)
    {
        m_state_code = bd + 1;
    }
    else if(m_bullet_hit_count == 2)
    {
        int sum_square = (m_state_code - 1) * (m_state_code - 1) + bd * bd;
        if(sum_square % 2 == 1)
        {
            m_state_code = ((double)sum_square + 19.0) / 4.0;
        }
        else
        {
            m_state_code = 9;
            to_erase = true;
        }
    }
    else
    {
        m_state_code = 9;
        to_erase = true;
    }

    switch(m_state_code)
    {
    case 1:
        collision_rect.x = pos_x;
        collision_rect.y = pos_y;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w;
        break;
    case 2:
        collision_rect.x = pos_x + m_sprite->rect.w / 2;
        collision_rect.y = pos_y;
        collision_rect.h = m_sprite->rect.h;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 3:
        collision_rect.x = pos_x;
        collision_rect.y = pos_y +  m_sprite->rect.h / 2;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w;
        break;
    case 4:
        collision_rect.x = pos_x;
        collision_rect.y = pos_y;
        collision_rect.h = m_sprite->rect.h;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 5:
        collision_rect.x = pos_x + m_sprite->rect.w / 2;
        collision_rect.y = pos_y;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 6:
        collision_rect.x = pos_x + m_sprite->rect.w / 2;
        collision_rect.y = pos_y + m_sprite->rect.h / 2;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 7:
        collision_rect.x = pos_x;
        collision_rect.y = pos_y;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 8:
        collision_rect.x = pos_x;
        collision_rect.y = pos_y + m_sprite->rect.h / 2;
        collision_rect.h = m_sprite->rect.h / 2;
        collision_rect.w = m_sprite->rect.w / 2;
        break;
    case 9:
        collision_rect.x = 0;
        collision_rect.y = 0;
        collision_rect.h = 0;
        collision_rect.w = 0;
        break;
    }

    src_rect = moveRect(Rect{m_sprite->rect.x, m_sprite->rect.y, m_sprite->rect.w, m_sprite->rect.h}, 0, m_state_code);
}
