#include "tank.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include <algorithm>

Tank::Tank(double x, double y, SpriteType type, InteractiveComponents interactive_components)
    : Object(x, y, type),
      InteractiveComponentsHolder(interactive_components)
{
    m_moving_direction = D_UP;
    m_tank_direction = D_UP;
    m_slip_time = 0;
    m_max_speed = AppConfig::tank_default_speed;
    m_speed = 0.0;
    m_shield = nullptr;
    m_boat = nullptr;
    m_shield_time = 0;
    m_frozen_time = 0;

    m_lives_count = 1;
    m_armor_count = 1;
}

Tank::~Tank()
{
    for (auto bullet : bullets)
        delete bullet;
    bullets.clear();

    if (m_shield != nullptr)
    {
        delete m_shield;
        m_shield = nullptr;
    }
    if (m_boat != nullptr)
    {
        delete m_boat;
        m_boat = nullptr;
    }
}

void Tank::draw(Renderer &renderer)
{
    if (to_erase)
        return;
    Object::draw(renderer);

    if (testFlag(TSF_SHIELD) && m_shield != nullptr)
        m_shield->draw(renderer);
    if (testFlag(TSF_BOAT) && m_boat != nullptr)
        m_boat->draw(renderer);

    for (auto bullet : bullets)
        if (bullet != nullptr)
            bullet->draw(renderer);
}

void Tank::update(Uint32 dt)
{
    if (to_erase)
        return;

    if (m_lives_count == 0 && bullets.size() == 0)
    {
        to_erase = true;
        return;
    }

    if (testFlag(TSF_ALIVE))
    {
        if (!m_blocked && !testFlag(TSF_FROZEN))
        {
            switch (m_moving_direction)
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
        }

        dest_rect.x = pos_x;
        dest_rect.y = pos_y;
        dest_rect.h = m_sprite->rect.h;
        dest_rect.w = m_sprite->rect.w;

        collision_rect.x = dest_rect.x + 2;
        collision_rect.y = dest_rect.y + 2;
        collision_rect.h = dest_rect.h - 4;
        collision_rect.w = dest_rect.w - 4;
    }

    if (testFlag(TSF_ON_ICE) && m_slip_time < AppConfig::slip_time)
    {
        m_slip_time += dt;
        if (m_slip_time >= AppConfig::slip_time)
        {
            m_moving_direction = m_tank_direction;
        }
    }

    if (testFlag(TSF_SHIELD) && m_shield != nullptr)
    {
        m_shield_time += dt;
        m_shield->pos_x = pos_x;
        m_shield->pos_y = pos_y;
        m_shield->update(dt);
        if (m_shield_time > AppConfig::tank_shield_time)
            clearFlag(TSF_SHIELD);
    }
    if (testFlag(TSF_BOAT) && m_boat != nullptr)
    {
        m_boat->pos_x = pos_x;
        m_boat->pos_y = pos_y;
        m_boat->update(dt);
    }
    if (testFlag(TSF_FROZEN))
    {
        m_frozen_time += dt;
        if (m_frozen_time > AppConfig::tank_frozen_time)
            clearFlag(TSF_FROZEN);
    }

    if (m_sprite->frames_count > 1 && (testFlag(TSF_ALIVE) && !testFlag(TSF_PREVIEW) ? m_speed > 0 : true))
    {
        m_frame_display_time += dt;
        if (m_frame_display_time > (testFlag(TSF_PREVIEW) ? m_sprite->frame_duration / 2 : m_sprite->frame_duration))
        {
            m_frame_display_time = 0;
            m_current_frame++;
            if (m_current_frame >= m_sprite->frames_count)
            {
                if (m_sprite->loop)
                    m_current_frame = 0;
                else if (testFlag(TSF_CREATING))
                {
                    // After CREATING state a tank goes to ALIVE state
                    m_sprite = &SpriteConfig::getInstance().getSpriteData(type);
                    clearFlag(TSF_CREATING);
                    setFlag(TSF_ALIVE);
                    m_current_frame = 0;
                }
                else if (testFlag(TSF_DESTROYED))
                {
                    m_current_frame = m_sprite->frames_count;

                    // After DESTROYED decreasing lives

                    if (m_lives_count == 1)
                    {
                        m_lives_count = 0;
                    }
                    else if (m_lives_count > 1)
                    {
                        m_lives_count--;
                        respawn();
                    }
                }
            }
        }
    }

    for (auto bullet : bullets)
        bullet->update(dt);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet *b)
                                 {if(b->to_erase) {delete b; return true;} return false; }),
                  bullets.end());
}

Bullet *Tank::fire()
{
    if (!testFlag(TSF_ALIVE))
        return nullptr;
    if (bullets.size() < m_bullet_max_count)
    {
        Direction tmp_d = (testFlag(TSF_ON_ICE) ? m_tank_direction : m_moving_direction);

        Bullet *bullet = new Bullet(this, tmp_d, AppConfig::bullet_default_speed);
        bullets.push_back(bullet);

        bullet->update(0); // recaulculate dest_rect
        return bullet;
    }
    return nullptr;
}

Rect Tank::nextCollisionRect(Uint32 dt)
{
    if (m_speed == 0)
        return collision_rect;

    Rect r;
    int a = 1;
    switch (m_moving_direction)
    {
    case D_UP:
        r.x = collision_rect.x;
        r.y = collision_rect.y - m_speed * dt - a;
        break;
    case D_RIGHT:
        r.x = collision_rect.x + m_speed * dt + a;
        r.y = collision_rect.y;
        break;
    case D_DOWN:
        r.x = collision_rect.x;
        r.y = collision_rect.y + m_speed * dt + a;
        break;
    case D_LEFT:
        r.x = collision_rect.x - m_speed * dt - a;
        r.y = collision_rect.y;
        break;
    }
    r.h = collision_rect.h;
    r.w = collision_rect.w;

    return r;
}

void Tank::setDirection(Direction d)
{
    if (!(testFlag(TSF_ALIVE) || testFlag(TSF_CREATING)))
        return;
    if (testFlag(TSF_ON_ICE))
    {
        m_tank_direction = d;
        if (m_speed == 0.0 || m_blocked || m_slip_time >= AppConfig::slip_time)
            m_moving_direction = d;
        if (m_slip_time != 0 && m_moving_direction == m_tank_direction)
            m_slip_time = 0;
    }
    else
    {
        m_tank_direction = d;
        m_moving_direction = d;
    }

    if (!m_blocked)
    {
        double epsilon = 5;
        int pos_x_tile, pos_y_tile;
        switch (m_moving_direction)
        {
        case D_UP:
        case D_DOWN:
            pos_x_tile = ((int)(pos_x / AppConfig::tile_size.w)) * AppConfig::tile_size.w;
            if (pos_x - pos_x_tile < epsilon)
                pos_x = pos_x_tile;
            else if (pos_x_tile + AppConfig::tile_size.w - pos_x < epsilon)
                pos_x = pos_x_tile + AppConfig::tile_size.w;
            break;
        case D_RIGHT:
        case D_LEFT:
            pos_y_tile = ((int)(pos_y / AppConfig::tile_size.h)) * AppConfig::tile_size.h;
            if (pos_y - pos_y_tile < epsilon)
                pos_y = pos_y_tile;
            else if (pos_y_tile + AppConfig::tile_size.h - pos_y < epsilon)
                pos_y = pos_y_tile + AppConfig::tile_size.h;
            break;
        }
    }
}

void Tank::collide(Rect &intersect_rect)
{
    if (intersect_rect.w > intersect_rect.h) // collision on top or bottom
    {
        if ((m_moving_direction == D_UP && intersect_rect.y <= collision_rect.y) ||
            (m_moving_direction == D_DOWN && (intersect_rect.y + intersect_rect.h) >= (collision_rect.y + collision_rect.h)))
        {
            m_blocked = true;
            m_slip_time = 0;
        }
    }
    else
    {
        if ((m_moving_direction == D_LEFT && intersect_rect.x <= collision_rect.x) ||
            (m_moving_direction == D_RIGHT && (intersect_rect.x + intersect_rect.w) >= (collision_rect.x + collision_rect.w)))
        {
            m_blocked = true;
            m_slip_time = 0;
        }
    }
}

void Tank::destroy()
{
    if (!testFlag(TSF_ALIVE))
        return;

    m_blocked = true;
    m_flags = TSF_DESTROYED;

    m_frame_display_time = 0;
    m_current_frame = 0;
    m_moving_direction = D_UP;
    m_speed = 0;
    m_slip_time = 0;
    m_sprite = &SpriteConfig::getInstance().getSpriteData(ST_DESTROY_TANK);

    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;

    dest_rect.x = pos_x + (dest_rect.w - m_sprite->rect.w) / 2;
    dest_rect.y = pos_y + (dest_rect.h - m_sprite->rect.h) / 2;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;
}

void Tank::setFlag(TankStateFlag flag)
{
    if (!testFlag(flag) && flag == TSF_ON_ICE)
        m_tank_direction = m_moving_direction;

    if (flag == TSF_SHIELD)
    {
        if (m_shield == nullptr)
            m_shield = new Object(pos_x, pos_y, ST_SHIELD);
        m_shield_time = 0;
    }
    if (flag == TSF_BOAT)
    {
        if (m_boat == nullptr)
            m_boat = new Object(pos_x, pos_y, type == ST_PLAYER_1 ? ST_BOAT_P1 : ST_BOAT_P2);
    }
    if (flag == TSF_FROZEN)
    {
        m_frozen_time = 0;
    }
    m_flags |= flag;
}

void Tank::clearFlag(TankStateFlag flag)
{
    if (flag == TSF_SHIELD)
    {
        if (m_shield != nullptr)
            delete m_shield;
        m_shield = nullptr;
        m_shield_time = 0;
    }
    if (flag == TSF_BOAT)
    {
        if (m_boat != nullptr)
            delete m_boat;
        m_boat = nullptr;
    }
    if (flag == TSF_ON_ICE)
    {
        m_moving_direction = m_tank_direction;
        m_slip_time = 0;
    }
    if (flag == TSF_FROZEN)
    {
        m_frozen_time = 0;
    }
    m_flags &= ~flag;
}

bool Tank::testFlag(TankStateFlag flag) const
{
    return (m_flags & flag) == flag;
}

void Tank::respawn()
{
    if (m_lives_count == 0)
    {
        to_erase = true;
        return;
    }
    creatingState();
}

void Tank::creatingState()
{
    m_sprite = &SpriteConfig::getInstance().getSpriteData(ST_CREATE);
    m_speed = 0.0;
    m_blocked = false;
    m_slip_time = 0;

    clearFlag(TSF_SHIELD);
    clearFlag(TSF_BOAT);
    m_flags = TSF_CREATING;

    dest_rect = Rect{(int)pos_x, (int)pos_y, m_sprite->rect.w, m_sprite->rect.h};
    collision_rect = Rect{0, 0, 0, 0};
}

Point Tank::center() const
{
    return {pos_x + dest_rect.w / 2, pos_y + dest_rect.h / 2};
}

bool Tank::alive() const
{
    return testFlag(TSF_ALIVE) && m_lives_count > 0 && m_armor_count > 0;
}

bool Tank::blocked() const
{
    return m_blocked;
}

Direction Tank::movingDirection() const
{
    return m_moving_direction;
}