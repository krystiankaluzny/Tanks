#include "tank_effect.h"
#include "tank.h"

TankEffect::TankEffect(unsigned id, Tank* tank, Object *object, Uint32 treshold_time)
    : id(id),
      to_erase(false),
      m_tank(tank),
      m_object(object),
      m_time(0),
      m_treshold_time(treshold_time)
{
}

TankEffect::~TankEffect()
{
    if (m_object != nullptr)
        delete m_object;
    m_object = nullptr;
}

void TankEffect::draw(Renderer &renderer)
{
    if (to_erase)
        return;
    if (m_object != nullptr)
        m_object->draw(renderer);
}

void TankEffect::update(Uint32 dt)
{
    if (to_erase)
        return;

    if (m_object != nullptr)
    {
        objectUpdate(dt);
    }

    m_time += dt;
    if (m_time >= m_treshold_time)
    {
        onTresholdReached();
    }
}

void TankEffect::resetTime()
{
    m_time = 0;
}

void TankEffect::objectUpdate(Uint32 dt)
{
    m_object->update(dt);
}

void TankEffect::onTresholdReached()
{
    // Default implementation does nothing
}