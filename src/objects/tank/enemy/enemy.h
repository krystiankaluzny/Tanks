#ifndef TANK_ENEMY_H
#define TANK_ENEMY_H

#include "../tank.h"

class Enemy : public Tank
{
public:
    Enemy(double x, double y, SpriteType type, unsigned armor_count, InteractiveComponents interactive_components);
    ~Enemy();

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;

    Bullet *fire() override;

    void hit();
    unsigned scoreForHit();

    void makeWithBonus();
    bool bonus() const;

    Point target_position;

private:
    void drawEnemy(Renderer &renderer);
    void updateBehavior(Uint32 dt);

    StateMachine *m_enemy_state_machine;

    bool m_bonus;

    Uint32 m_direction_time;
    Uint32 m_keep_direction_time;
    Uint32 m_speed_time;
    Uint32 m_try_to_go_time;

    Uint32 m_fire_time;
    Uint32 m_reload_time;

    // Sub-states
    class CreatingState : public ContextState<Enemy>
    {
    public:
        CreatingState(Enemy *enemy);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;

    protected:
        void onInitialize() override;
    };

    class AliveState : public ContextState<Enemy>
    {
    public:
        AliveState(Enemy *enemy);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;

    protected:
        void onInitialize() override;

    private:
        void checkKeyStates(const UpdateState &updateState);
    };

    class DestroyedState : public ContextState<Enemy>
    {
    public:
        DestroyedState(Enemy *enemy);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;

    protected:
        void onInitialize() override;
    };
};

#endif // TANK_ENEMY_H
