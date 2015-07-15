#include "tank.h"
#include "../appconfig.h"
#include <algorithm>

Tank::Tank()
    : Object(AppConfig::enemy_starting_point.at(0).x, AppConfig::enemy_starting_point.at(0).y, ST_TANK_A)
{
    direction = D_UP;
    m_slip_time = 0;
    default_speed = AppConfig::tank_default_speed;
    speed = 0.0;
    m_shield = nullptr;
    m_boat = nullptr;
    m_shield_time = 0;
    m_frozen_time = 0;
}

Tank::Tank(double x, double y, SpriteType type)
    : Object(x, y, type)
{
    direction = D_UP;
    m_slip_time = 0;
    default_speed = AppConfig::tank_default_speed;
    speed = 0.0;
    m_shield = nullptr;
    m_boat = nullptr;
    m_shield_time = 0;
    m_frozen_time = 0;
}

Tank::~Tank()
{
    for(auto bullet : bullets) delete bullet;
    bullets.clear();

    if(m_shield != nullptr)
    {
        delete m_shield;
        m_shield = nullptr;
    }
    if(m_boat != nullptr)
    {
        delete m_boat;
        m_boat = nullptr;
    }
}

void Tank::draw()
{
    if(to_erase) return;
    Object::draw();

    if(testFlag(TSF_SHIELD) && m_shield != nullptr) m_shield->draw();
    if(testFlag(TSF_BOAT) && m_boat != nullptr) m_boat->draw();

    for(auto bullet : bullets)
        if(bullet != nullptr) bullet->draw();
}

void Tank::update(Uint32 dt)
{
    if(to_erase) return;
    if(testFlag(TSF_LIFE))
    {
        if(!stop && !testFlag(TSF_FROZEN))
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

    if(testFlag(TSF_ON_ICE) && m_slip_time > 0)
    {
        m_slip_time -= dt;
        if(m_slip_time <= 0)
        {
            clearFlag(TSF_ON_ICE);
            m_slip_time = 0;
            direction = new_direction;
        }
    }

    if(testFlag(TSF_SHIELD) && m_shield != nullptr)
    {
        m_shield_time += dt;
        m_shield->pos_x = pos_x;
        m_shield->pos_y = pos_y;
        m_shield->update(dt);
        if(m_shield_time > AppConfig::tank_shield_time) clearFlag(TSF_SHIELD);
    }
    if(testFlag(TSF_BOAT) && m_boat != nullptr)
    {
        m_boat->pos_x = pos_x;
        m_boat->pos_y = pos_y;
        m_boat->update(dt);
    }
    if(testFlag(TSF_FROZEN))
    {
        m_frozen_time += dt;
        if(m_frozen_time > AppConfig::tank_frozen_time) clearFlag(TSF_FROZEN);
    }

    if(m_sprite->frames_count > 1 && (testFlag(TSF_LIFE) ? speed > 0 : true)) //brak animacji jeśli czołg nie prógbuje jechać
    {
        m_frame_display_time += dt;
        if(m_frame_display_time > (testFlag(TSF_MENU)  ? m_sprite->frame_duration / 2 : m_sprite->frame_duration))
        {
            m_frame_display_time = 0;
            m_current_frame++;
            if(m_current_frame >= m_sprite->frames_count)
            {
                if(m_sprite->loop) m_current_frame = 0;
                else if(testFlag(TSF_CREATE))
                {
                    m_sprite = Engine::getEngine().getSpriteConfig()->getSpriteData(type);
                    clearFlag(TSF_CREATE);
                    setFlag(TSF_LIFE);
                    m_current_frame = 0;
                }
                else if(testFlag(TSF_DESTROYED))
                {
                    m_current_frame = m_sprite->frames_count;
                    if(lives_count > 0) respawn();
                    else if(bullets.size() == 0) to_erase = true;
                }
            }
        }
    }


    // Obsługa pocisku
    for(auto bullet : bullets) bullet->update(dt);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet*b){if(b->to_erase) {delete b; return true;} return false;}), bullets.end());
}

Bullet* Tank::fire()
{
    if(!testFlag(TSF_LIFE)) return nullptr;
    if(bullets.size() < m_bullet_max_size)
    {
        //podajemy początkową dowolną pozycję, bo nie znamy wymiarów pocisku
        Bullet* bullet = new Bullet(pos_x, pos_y);
        bullets.push_back(bullet);

        Direction tmp_d = (testFlag(TSF_ON_ICE) ? new_direction : direction);
        switch(tmp_d)
        {
        case D_UP:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y -= bullet->dest_rect.h - 4;
            break;
        case D_RIGHT:
            bullet->pos_x += dest_rect.w - 4;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        case D_DOWN:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y += dest_rect.h - 4;
            break;
        case D_LEFT:
            bullet->pos_x -= bullet->dest_rect.w - 4;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        }

        bullet->direction = tmp_d;
        if(type == ST_TANK_C)
            bullet->speed = AppConfig::bullet_default_speed * 1.3;
        else
            bullet->speed = AppConfig::bullet_default_speed;

        bullet->update(0); //zmiana pozycji dest_rect
        return bullet;
    }
    return nullptr;
}

SDL_Rect Tank::nextCollisionRect(Uint32 dt)
{
    if(speed == 0) return collision_rect;

    SDL_Rect r;
    int a = 1;
    switch (direction)
    {
    case D_UP:
        r.x = collision_rect.x;
        r.y = collision_rect.y - default_speed * dt - a;
        break;
    case D_RIGHT:
        r.x = collision_rect.x + default_speed * dt + a;
        r.y = collision_rect.y;
        break;
    case D_DOWN:
        r.x = collision_rect.x;
        r.y = collision_rect.y + default_speed * dt + a;
        break;
    case D_LEFT:
        r.x = collision_rect.x - default_speed * dt - a;
        r.y = collision_rect.y;
        break;
    }
    r.h = collision_rect.h;
    r.w = collision_rect.w;

    return r;
}

void Tank::setDirection(Direction d)
{
    if(!(testFlag(TSF_LIFE) || testFlag(TSF_CREATE))) return;
    if(testFlag(TSF_ON_ICE))
    {
        new_direction = d;
        if(speed == 0.0 || m_slip_time == 0.0) direction = d;
        if((m_slip_time != 0 && direction == new_direction) || m_slip_time == 0)
            m_slip_time = AppConfig::slip_time;
    }
    else
        direction = d;

    if(!stop)
    {
        double epsilon = 5;
        int pos_x_tile, pos_y_tile;
        switch (direction)
        {
        case D_UP:
        case D_DOWN:
            pos_x_tile = ((int)(pos_x / AppConfig::tile_rect.w)) * AppConfig::tile_rect.w;
            if(pos_x - pos_x_tile < epsilon) pos_x = pos_x_tile;
            else if(pos_x_tile + AppConfig::tile_rect.w - pos_x < epsilon) pos_x = pos_x_tile + AppConfig::tile_rect.w;
            break;
        case D_RIGHT:
        case D_LEFT:
            pos_y_tile = ((int)(pos_y / AppConfig::tile_rect.h)) * AppConfig::tile_rect.h;
            if(pos_y - pos_y_tile < epsilon) pos_y = pos_y_tile;
            else if(pos_y_tile + AppConfig::tile_rect.h - pos_y < epsilon) pos_y = pos_y_tile + AppConfig::tile_rect.h;
            break;
        }
    }
}

void Tank::collide(SDL_Rect &intersect_rect)
{
    if(intersect_rect.w > intersect_rect.h) // kolizja od góry lub dołu
    {
        if((direction == D_UP && intersect_rect.y <= collision_rect.y) ||
                (direction == D_DOWN && (intersect_rect.y + intersect_rect.h) >= (collision_rect.y + collision_rect.h)))
        {
            stop = true;
            m_slip_time = 0;
        }
    }
    else
    {
        if((direction == D_LEFT && intersect_rect.x <= collision_rect.x) ||
                (direction == D_RIGHT && (intersect_rect.x + intersect_rect.w) >= (collision_rect.x + collision_rect.w)))
        {
            stop = true;
            m_slip_time = 0;
        }
    }
}

void Tank::destroy()
{
    if(!testFlag(TSF_LIFE)) return;

    stop = true;
    m_flags = TSF_DESTROYED;

    m_frame_display_time = 0;
    m_current_frame = 0;
    direction = D_UP;
    speed = 0;
    m_slip_time = 0;
    m_sprite = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_DESTROY_TANK);

    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;

    dest_rect.x = pos_x + (dest_rect.w - m_sprite->rect.w)/2;
    dest_rect.y = pos_y + (dest_rect.h - m_sprite->rect.h)/2;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;
}

void Tank::setFlag(TankStateFlag flag)
{
    if(!testFlag(flag) && flag == TSF_ON_ICE)
        new_direction = direction;

    if(flag == TSF_SHIELD)
    {
        if(m_shield == nullptr) m_shield = new Object(pos_x, pos_y, ST_SHIELD);
         m_shield_time = 0;
    }
    if(flag == TSF_BOAT)
    {
         if(m_boat == nullptr) m_boat = new Object(pos_x, pos_y, type == ST_PLAYER_1 ? ST_BOAT_P1 : ST_BOAT_P2);
    }
    if(flag == TSF_FROZEN)
    {
        m_frozen_time = 0;
    }
    m_flags |= flag;
}

void Tank::clearFlag(TankStateFlag flag)
{
    if(flag == TSF_SHIELD)
    {
         if(m_shield != nullptr) delete m_shield;
         m_shield = nullptr;
         m_shield_time = 0;
    }
    if(flag == TSF_BOAT)
    {
         if(m_boat != nullptr) delete m_boat;
         m_boat = nullptr;
    }
    if(flag == TSF_FROZEN)
    {
        m_frozen_time = 0;
    }
    m_flags &= ~flag;
}

bool Tank::testFlag(TankStateFlag flag)
{
    return (m_flags & flag) == flag;
}

void Tank::respawn()
{
    m_sprite = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_CREATE);
    speed = 0.0;
    stop = false;
    m_slip_time = 0;

    clearFlag(TSF_SHIELD);
    clearFlag(TSF_BOAT);
    m_flags = TSF_LIFE;
    update(0);
    m_flags = TSF_CREATE; //resetujemy wszystkie inne flagi

    //ustawienie porostokąta kolizji po wywołaniu update
    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;
}
