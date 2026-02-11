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
    m_max_speed = AppConfig::tank_default_speed;
    m_speed = 0.0;
    m_frozen = false;
    m_blocked = false;

    m_lives_count = 1;
    m_armor_count = 1;
}

Tank::~Tank()
{
    for (auto effect : m_effects)
        delete effect;
    m_effects.clear();

    for (auto bullet : bullets)
        delete bullet;
    bullets.clear();
}

void Tank::draw(Renderer &renderer)
{
    if (to_erase)
        return;
    Object::draw(renderer);

    for (auto effect : m_effects)
        effect->draw(renderer);

    for (auto bullet : bullets)
        bullet->draw(renderer);
}

void Tank::update(Uint32 dt)
{
}

Bullet *Tank::fire()
{
    if (bullets.size() < m_bullet_max_count)
    {
        Bullet *bullet = new Bullet(this, m_tank_direction, AppConfig::bullet_default_speed);
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
    OnIceEffect *onIceEffect = dynamic_cast<OnIceEffect *>(findEffectByType(TankEffectType::ON_ICE));
    if (onIceEffect != nullptr)
    {
        m_tank_direction = d;
        if (m_speed == 0.0 || m_blocked || !onIceEffect->isSlipping())
            m_moving_direction = d;
        if (onIceEffect->isSlipping() && m_moving_direction == m_tank_direction)
            onIceEffect->resetTime();
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
            resetSlipEffect();
        }
    }
    else
    {
        if ((m_moving_direction == D_LEFT && intersect_rect.x <= collision_rect.x) ||
            (m_moving_direction == D_RIGHT && (intersect_rect.x + intersect_rect.w) >= (collision_rect.x + collision_rect.w)))
        {
            m_blocked = true;
            resetSlipEffect();
        }
    }
}

Point Tank::center() const
{
    return {pos_x + dest_rect.w / 2, pos_y + dest_rect.h / 2};
}

bool Tank::alive() const
{
    return m_lives_count > 0 && m_armor_count > 0;
}

bool Tank::blocked() const
{
    return m_blocked;
}

Direction Tank::movingDirection() const
{
    return m_moving_direction;
}

void Tank::activateShieldEffect()
{
    deleteEffectByType(Tank::TankEffectType::SHIELD);
    m_effects.push_back(new ShieldEffect(this));
}

void Tank::activateShortShieldEffect()
{
    deleteEffectByType(Tank::TankEffectType::SHIELD);
    m_effects.push_back(new ShortShieldEffect(this));
}

void Tank::activateBoatEffect()
{
    deleteEffectByType(Tank::TankEffectType::BOAT);
    m_effects.push_back(new BoatEffect(this));
}

void Tank::activateFrozenEffect()
{
    deleteEffectByType(Tank::TankEffectType::FROZEN);
    m_effects.push_back(new FrozenEffect(this));
}

void Tank::activateOnIceEffect()
{
    OnIceEffect *onIceEffect = dynamic_cast<OnIceEffect *>(findEffectByType(TankEffectType::ON_ICE));
    if (onIceEffect != nullptr)
    {
        onIceEffect->resetTime();
    }
    else
    {
        m_effects.push_back(new OnIceEffect(this));
    }
}

void Tank::deactivateOnIceEffect()
{
    deleteEffectByType(Tank::TankEffectType::ON_ICE);
}

void Tank::deactivateAllEffects()
{
    std::for_each(m_effects.begin(), m_effects.end(), [](TankEffect *e)
                  { e->to_erase = true; });
}

bool Tank::hasBoatEffect()
{
    return hasEffectByType(Tank::TankEffectType::BOAT);
}

void Tank::deleteEffectByType(TankEffectType type)
{
    std::for_each(m_effects.begin(), m_effects.end(), [type](TankEffect *e)
                  { if (e->id == type) e->to_erase = true; });
}

bool Tank::hasEffectByType(TankEffectType type) const
{
    auto it = std::find_if(m_effects.begin(), m_effects.end(), [type](TankEffect *e)
                           { return e->id == type; });
    return it != m_effects.end();
}

TankEffect *Tank::findEffectByType(TankEffectType type)
{
    auto it = std::find_if(m_effects.begin(), m_effects.end(), [type](TankEffect *e)
                           { return e->id == type; });
    if (it != m_effects.end())
        return *it;
    return nullptr;
}

void Tank::updatePosition(Uint32 dt)
{
    if (!m_blocked && !m_frozen)
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

void Tank::updateBullets(Uint32 dt)
{
    for (auto bullet : bullets)
        bullet->update(dt);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet *b)
                                 {if(b->to_erase) {delete b; return true;} return false; }),
                  bullets.end());
}

void Tank::updateEffects(Uint32 dt)
{
    for (auto effect : m_effects)
        effect->update(dt);

    m_effects.erase(std::remove_if(m_effects.begin(), m_effects.end(), [](TankEffect *e)
                                   {if (e->to_erase) {delete e; return true;} return false; }),
                    m_effects.end());
}

bool Tank::isSlipping()
{

    OnIceEffect *onIceEffect = dynamic_cast<OnIceEffect *>(findEffectByType(TankEffectType::ON_ICE));
    if (onIceEffect != nullptr)
    {
        return onIceEffect->isSlipping();
    }

    return false;
}

void Tank::resetSlipEffect()
{
    OnIceEffect *onIceEffect = dynamic_cast<OnIceEffect *>(findEffectByType(TankEffectType::ON_ICE));
    if (onIceEffect != nullptr)
    {
        onIceEffect->resetTime();
    }
}

// ------- Effects implementations -------
// --------------------------------------
Tank::ShieldEffect::ShieldEffect(Tank *tank)
    : TankEffect(Tank::TankEffectType::SHIELD,
                 tank,
                 new Object(tank->pos_x, tank->pos_y, ST_SHIELD),
                 AppConfig::tank_shield_time)
{
}

void Tank::ShieldEffect::objectUpdate(Uint32 dt)
{
    m_object->pos_x = m_tank->pos_x;
    m_object->pos_y = m_tank->pos_y;
    m_object->update(dt);
}

void Tank::ShieldEffect::onTresholdReached()
{
    to_erase = true;
}

// --------------------------------------

Tank::ShortShieldEffect::ShortShieldEffect(Tank *tank)
    : TankEffect(Tank::TankEffectType::SHIELD,
                 tank,
                 new Object(tank->pos_x, tank->pos_y, ST_SHIELD),
                 AppConfig::tank_shield_time / 2)
{
}

void Tank::ShortShieldEffect::objectUpdate(Uint32 dt)
{
    m_object->pos_x = m_tank->pos_x;
    m_object->pos_y = m_tank->pos_y;
    m_object->update(dt);
}

void Tank::ShortShieldEffect::onTresholdReached()
{
    to_erase = true;
}

// --------------------------------------
Tank::BoatEffect::BoatEffect(Tank *tank)
    : TankEffect(Tank::TankEffectType::BOAT,
                 tank,
                 new Object(tank->pos_x, tank->pos_y, tank->type == ST_PLAYER_1 ? ST_BOAT_P1 : ST_BOAT_P2),
                 0)
{
}

void Tank::BoatEffect::objectUpdate(Uint32 dt)
{
    m_object->pos_x = m_tank->pos_x;
    m_object->pos_y = m_tank->pos_y;
    m_object->update(dt);
}

// --------------------------------------

Tank::FrozenEffect::FrozenEffect(Tank *tank)
    : TankEffect(Tank::TankEffectType::FROZEN,
                 tank,
                 nullptr,
                 AppConfig::tank_frozen_time)
{
    m_tank->m_frozen = true;
}

void Tank::FrozenEffect::onTresholdReached()
{
    m_tank->m_frozen = false;
    to_erase = true;
}

// --------------------------------------
Tank::OnIceEffect::OnIceEffect(Tank *tank)
    : TankEffect(Tank::TankEffectType::ON_ICE,
                 tank,
                 nullptr,
                 AppConfig::slip_time)
{
}

void Tank::OnIceEffect::onTresholdReached()
{
    m_tank->m_moving_direction = m_tank->m_tank_direction;
}

bool Tank::OnIceEffect::isSlipping() const
{
    return m_time > 0 && m_time < m_treshold_time;
}