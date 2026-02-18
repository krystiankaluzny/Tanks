#ifndef GAME_H
#define GAME_H
#include "../appstate.h"
#include "../../objects/object.h"
#include "../../objects/tank/player/player.h"
#include "../../objects/tank/enemy/enemy.h"
#include "../../objects/tank/bullet.h"
#include "../../objects/brick.h"
#include "../../objects/eagle.h"
#include "../../objects/bonus.h"
#include "../../engine/state_machine/context_state.h"
#include "level_environment.h"
#include <vector>
#include <string>

class Game : public AppState
{
public:
    Game(int players_count, InteractiveComponents interactive_components, StateMachine *state_machine);
    Game(std::vector<Player *> players, int previous_level, InteractiveComponents interactive_components, StateMachine *state_machine);
    ~Game();

    void draw(Renderer &renderer) override;
    void update(const UpdateState &updateState) override;

    /**
     * Key press handling function.
     * Enters - pause mode,
     * ESC - returns to menu,
     * N - jump to next level
     * B - jump to previous level
     * T - show enemy target paths
     */
    void eventProcess(const Event &event) override;

private:
    void clearAll();
    void createPlayersIfNeeded();

    void drawScene(Renderer &renderer);
    void drawObjects(Renderer &renderer);
    void drawEnemy(Renderer &renderer, Enemy *enemy);
    void drawGameOver(Renderer &renderer);
    void drawGameStatusPanel(Renderer &renderer);

    void updateScene(Uint32 dt);
    void checkCollisions(Uint32 dt);
    void checkCollisionTwoTanks(Tank *tank1, Tank *tank2, Uint32 dt);
    void checkCollisionPlayerBulletsWithEnemy(Player *player, Enemy *enemy);
    void checkCollisionEnemyBulletsWithPlayer(Enemy *enemy, Player *player);
    void checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2);
    void checkCollisionPlayerWithBonus(Player *player, Bonus *bonus);

    void updateObjects(Uint32 dt);

    void calculateEnemiesTargets();
    void generateEnemyIfPossible(Uint32 dt);

    void generateBonus();

    void transiteToGameOver();

    StateMachine *m_game_state_machine;
    LevelEnvironment *m_level_environment;

    std::vector<Enemy *> m_enemies;
    std::vector<Player *> m_players;
    std::vector<Player *> m_killed_players;
    std::vector<Bonus *> m_bonuses;

    int m_current_level;
    int m_players_count;
    int m_enemies_to_kill_count;

    Uint32 m_new_enemy_cooldown;
    unsigned m_enemy_respown_position;

    bool m_show_enemies_targets;

    // Sub-states
    class StartingState : public ContextState<Game>
    {
    public:
        StartingState(Game *ps);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;

    private:
        Uint32 m_level_start_time;
    };

    class PlayingState : public ContextState<Game>
    {
    public:
        PlayingState(Game *ps);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;
    };

    class LevelEndingState : public ContextState<Game>
    {
    public:
        LevelEndingState(Game *ps, bool no_waiting, bool game_over);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;

    private:
        Uint32 m_level_end_time;
        bool m_no_waiting;
        bool m_game_over;
    };

    class PauseState : public ContextState<Game>
    {
    public:
        PauseState(Game *ps);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;

    protected:
        void onInitialize() override;
    };

    class GameOverState : public ContextState<Game>
    {
    public:
        GameOverState(Game *ps);
        void draw(Renderer &renderer) override;
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;

    protected:
        void onInitialize() override;

    private:
        double m_game_over_message_position;
    };
};

#endif // GAME_H
