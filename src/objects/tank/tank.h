#ifndef TANK_H
#define TANK_H

#include "../object.h"
#include "bullet.h"
#include "tank_effect.h"
#include "../../engine/state_machine/context_state.h"

#include <vector>

typedef unsigned TankStateFlags;

class Tank : public Object, protected InteractiveComponentsHolder
{
public:
    Tank(double x, double y, SpriteType type, InteractiveComponents interactive_components);
    virtual ~Tank();

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;
    virtual Bullet *fire();

    Rect nextCollisionRect(Uint32 dt);
    void setDirection(Direction d);
    void collide(Rect &intersect_rect);

    Point center() const;
    bool alive() const;
    bool blocked() const;
    Direction movingDirection() const;

    void activateShieldEffect();
    void activateShortShieldEffect();
    void activateBoatEffect();
    void activateFrozenEffect();
    void activateOnIceEffect();

    void deactivateOnIceEffect();
    void deactivateAllEffects();

    bool hasBoatEffect();

    std::vector<Bullet *> bullets;

protected:
    enum TankEffectType
    {
        SHIELD = 1,
        BOAT = 2,
        FROZEN = 3,
        ON_ICE = 4,
    };

    void deleteEffectByType(TankEffectType type);
    bool hasEffectByType(TankEffectType type) const;
    TankEffect *findEffectByType(TankEffectType type);
    void updatePosition(Uint32 dt);
    void updateBullets(Uint32 dt);
    void updateEffects(Uint32 dt);

    bool isSlipping();
    void resetSlipEffect();

    double m_max_speed;
    double m_speed;
    bool m_frozen;
    bool m_blocked;

    Direction m_tank_direction;
    Direction m_moving_direction;

    TankStateFlags m_flags;

    unsigned m_lives_count;
    unsigned m_armor_count;

    std::vector<TankEffect *> m_effects;

    unsigned m_bullet_max_count;

    // Effects
    class ShieldEffect : public TankEffect
    {
    public:
        ShieldEffect(Tank *tank);
        void objectUpdate(Uint32 dt) override;
        void onTresholdReached() override;
    };

    class ShortShieldEffect : public TankEffect
    {
    public:
        ShortShieldEffect(Tank *tank);
        void objectUpdate(Uint32 dt) override;
        void onTresholdReached() override;
    };

    class BoatEffect : public TankEffect
    {
    public:
        BoatEffect(Tank *tank);
        void objectUpdate(Uint32 dt) override;
    };

    class FrozenEffect : public TankEffect
    {
    public:
        FrozenEffect(Tank *tank);
        void onTresholdReached() override;
    };

    class OnIceEffect : public TankEffect
    {
    public:
        OnIceEffect(Tank *tank);
        void onTresholdReached() override;
        bool isSlipping() const;
    };
};

#endif // TANK_H
