#ifndef TANK_PLAYER_H
#define TANK_PLAYER_H

#include "../tank.h"
#include <vector>

class Player : public Tank
{
public:
    Player(double x, double y, SpriteType type, std::vector<KeyCode> control_keys, InteractiveComponents interactive_components);
    ~Player();

    void draw(Renderer &renderer) override;
    void handleKeyboardEvent(const KeyboardEvent &ev);
    void update(Uint32 dt) override;

    void hit();
    void moveToNextLevel();
    void startPreview();
    /**
     * @return pointer to a new bullet, or nullptr if for some reason bullet cannot be created
     */
    Bullet *fire() override;
    void changeStarCountBy(int c);
    void addScore(unsigned points);
    unsigned score() const;

    void addLife();
    unsigned lives() const;

    double speed() const;

private:
    void drawPlayer(Renderer &renderer);
    void respawn();
    void resetKeyStates();

    StateMachine *m_player_state_machine;

    unsigned m_score;
    int star_count;
    Uint32 m_fire_time;

    struct KeyState
    {
        KeyCode key;
        bool pressed;
    };

    KeyState m_key_state_up;
    KeyState m_key_state_down;
    KeyState m_key_state_left;
    KeyState m_key_state_right;
    KeyState m_key_state_fire;

    // Sub-states
    class CreatingState : public ContextState<Player>
    {
    public:
        CreatingState(Player *player);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
    };

    class AliveState : public ContextState<Player>
    {
    public:
        AliveState(Player *player);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;

    private:
        void checkKeyStates(const UpdateState &updateState);
    };

    class DestroyedState : public ContextState<Player>
    {
    public:
        DestroyedState(Player *player);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
    };

    class PreviewState : public ContextState<Player>
    {
    public:
        PreviewState(Player *player);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
    };
};

#endif // TANK_PLAYER_H
