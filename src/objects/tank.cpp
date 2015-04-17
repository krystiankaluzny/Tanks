#include "tank.h"
#include "../appconfig.h"

Tank::Tank()
    : Object(AppConfig::player1_starting_point_x, AppConfig::player1_starting_point_y, ST_PLAYER_1)
{
    direction = D_UP;
    bullet = nullptr;
    respawn();
}

Tank::Tank(double x, double y, SpriteType type)
    : Object(x, y, type)
{
    direction = D_UP;
    bullet = nullptr;
    respawn();
}

Tank::~Tank()
{
    delete bullet;
}

void Tank::draw()
{
    if(m_sprite == nullptr) return;

    Engine::getEngine().getRenderer()->drawObject(&src_rect, &dest_rect);
    if(bullet != nullptr) bullet->draw();
}

void Tank::update(Uint32 dt)
{
    if(testFlag(TSF_LIFE))
    {
        if(!stop)
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

    if(m_sprite->frames_count > 1 && (testFlag(TSF_LIFE) ? speed > 0 : true)) //brak animacji jeśli czołg nie prógbuje jechać
    {
        m_frame_display_time += dt;
        if(m_frame_display_time > m_sprite->frame_duration)
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
                else if(testFlag(TSF_DESTROYED) && bullet == nullptr)
                {
                    //TODO uwzględnić życia
                 //       to_erase = true;
                    respawn();
                }
            }
        }
    }

    stop = false;
    if(testFlag(TSF_LIFE))
        src_rect = moveRect(m_sprite->rect, direction, m_current_frame);
    else
        src_rect = moveRect(m_sprite->rect, 0, m_current_frame);

    // Obsługa pocisku
    if(bullet != nullptr)
    {
        if(bullet->to_erase)
        {
            delete bullet;
            bullet = nullptr;
        }
        else bullet->update(dt);
    }
}

void Tank::fire()
{
    if(!testFlag(TSF_LIFE)) return;
    if(bullet == nullptr)
    {
        //podajemy początkową dowolną pozycję, bo nie znamy wymiarów pocisku
        bullet = dynamic_cast<Bullet*>(ObjectFactory::Create(pos_x, pos_y, ST_BULLET));

        switch(direction)
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

        bullet->direction = direction;
        bullet->speed = 0.3;
        bullet->update(0); //zmiana pozycji dest_rect
    }
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
    direction = d;

    if(!stop)
    {
        double epsilon = 5;
        int pos_x_tile, pos_y_tile;
        switch (direction)
        {
        case D_UP:
        case D_DOWN:
            pos_x_tile = ((int)(pos_x / AppConfig::tile_width)) * AppConfig::tile_width;
            if(pos_x - pos_x_tile < epsilon) pos_x = pos_x_tile;
            else if(pos_x_tile + AppConfig::tile_width - pos_x < epsilon) pos_x = pos_x_tile + AppConfig::tile_width;
            break;
        case D_RIGHT:
        case D_LEFT:
            pos_y_tile = ((int)(pos_y / AppConfig::tile_height)) * AppConfig::tile_height;
            if(pos_y - pos_y_tile < epsilon) pos_y = pos_y_tile;
            else if(pos_y_tile + AppConfig::tile_height - pos_y < epsilon) pos_y = pos_y_tile + AppConfig::tile_height;
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
        }
    }
    else
    {
        if((direction == D_LEFT && intersect_rect.x <= collision_rect.x) ||
           (direction == D_RIGHT && (intersect_rect.x + intersect_rect.w) >= (collision_rect.x + collision_rect.w)))
        {
            stop = true;
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
    m_flags |= flag;
}

void Tank::clearFlag(TankStateFlag flag)
{
    m_flags ^= flag;
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

    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;

    m_flags = TSF_LIFE;
    update(0);
    m_flags = TSF_CREATE; //resetujemy wszystkie inne flagi
}

