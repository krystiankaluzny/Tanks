#ifndef GAME_H
#define GAME_H
#include "../appstate.h"
#include "../../objects/object.h"
#include "../../objects/player.h"
#include "../../objects/enemy.h"
#include "../../objects/bullet.h"
#include "../../objects/brick.h"
#include "../../objects/eagle.h"
#include "../../objects/bonus.h"
#include "level_environment.h"
#include <vector>
#include <string>

class Game : public AppState
{
public:
    Game(int players_count);
    Game(std::vector<Player *> players, int previous_level);
    ~Game();

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;

    /**
     * Key press handling function.
     * Enters - pause mode,
     * ESC - returns to menu,
     * N - jump to next level
     * B - jump to previous level
     * T - show enemy target paths
     */
    void eventProcess(const Event &event) override;

    AppState *nextState() override;

private:
    void clearAll();
    void createPlayersIfNeeded();

    void drawLevelStartScreen(Renderer &renderer);
    void drawObjects(Renderer &renderer);
    void drawEnemy(Renderer &renderer, Enemy *enemy);
    void drawGameOver(Renderer &renderer);
    void drawGameStatusPanel(Renderer &renderer);

    void checkCollisions(Uint32 dt);
    void checkCollisionTwoTanks(Tank *tank1, Tank *tank2, Uint32 dt);
    void checkCollisionPlayerBulletsWithEnemy(Player *player, Enemy *enemy);
    void checkCollisionEnemyBulletsWithPlayer(Enemy *enemy, Player *player);
    void checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2);
    void checkCollisionPlayerWithBonus(Player *player, Bonus *bonus);

    void updateObjects(Uint32 dt);

    void gameOver();
    void calculateEnemiesTargets();

    void generateEnemyIfPossible(Uint32 dt);
    void generateBonus();

    LevelEnvironment *m_level_environment;

    std::vector<Enemy *> m_enemies;
    std::vector<Player *> m_players;
    std::vector<Player *> m_killed_players;
    std::vector<Bonus *> m_bonuses;

    int m_current_level;
    int m_players_count;
    int m_enemies_to_kill_count;

    bool m_level_start_screen;
    Uint32 m_level_start_time;
    Uint32 m_new_enemy_cooldown;
    Uint32 m_level_end_time;

    bool m_game_over;
    double m_game_over_message_position;
    bool m_finished;
    bool m_pause;
    unsigned m_enemy_respown_position;

    bool m_show_enemies_targets;
};

#endif // GAME_H
