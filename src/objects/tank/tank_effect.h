#ifndef TANK_EFFECT_H
#define TANK_EFFECT_H

#include "../object.h"

class Tank;

class TankEffect
{
public:
    TankEffect(Uint32 id, Tank* tank, Object* object, Uint32 treshold_time);
    virtual ~TankEffect();

    void draw(Renderer &renderer);
    void update(Uint32 dt);
    void resetTime();

    virtual void objectUpdate(Uint32 dt);
    virtual void onTresholdReached();

    Uint32 id;
    bool to_erase;

protected:
    Tank* m_tank;
    Object* m_object;
    Uint32 m_time;
    Uint32 m_treshold_time;
};

#endif // TANK_EFFECT_H