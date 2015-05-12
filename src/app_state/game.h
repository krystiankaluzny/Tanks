#ifndef GAME_H
#define GAME_H
#include "appstate.h"
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/enemy.h"
#include "../objects/bullet.h"
#include "../objects/brick.h"
#include "../objects/eagle.h"
#include "../objects/bonus.h"
#include <vector>
#include <string>

class Game : public AppState
{
public:
    Game();
    Game(int players_count);
    Game(std::vector<Player*> players, int previous_level);

    ~Game();
    bool finished() const;
    void draw();
    void update(Uint32 dt);
    void eventProcess(SDL_Event* ev);
    AppState* nextState();

private:
    void loadLevel(std::string path);
    void clearLevel();
    void nextLevel();
    void generateEnemy();
    void generateBonus();

    void checkCollisionTankWithLevel(Tank* tank, Uint32 dt);
    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt);
    void checkCollisionBulletWithLevel(Bullet* bullet);
    void checkCollisionBulletWithBush(Bullet* bullet);
    void checkCollisionPlayerBulletsWithEnemy(Player* player, Enemy* enemy);
    void checkCollisionEnemyBulletsWithPlayer(Enemy* enemy, Player* player);
    void checkCollisionTwoBullets(Bullet* bullet1, Bullet* bullet2);
    void checkCollisionPlayerWithBonus(Player* player, Bonus* bonus);

    int m_level_columns_count; // wymiary mapy pomocnicze zamiast używać rozmiarów wektora m_level
    int m_level_rows_count;
    std::vector< std::vector <Object*> > m_level; //level bez krzaków
    std::vector<Object*> m_bushes; //krzaki

    std::vector<Enemy*> m_enemies; //wrogowie
    std::vector<Player*> m_players; //gracze
    std::vector<Player*> m_killed_players; //zabici gracze
    std::vector<Bonus*> m_bonuses;
    Eagle* m_eagle;

    int m_current_level; //numer poziomu
    int m_player_count;  //gra na 1 gracza lub 2
    int m_enemy_to_kill; //wrogowie do zabicia

    bool m_level_start_screen;
    bool m_protect_eagle;
    Uint32 m_level_start_time;
    Uint32 m_enemy_redy_time; //czas od ostatniego stworzenia przeciwnika
    Uint32 m_level_end_time;
    Uint32 m_protect_eagle_time;

    bool m_game_over;   //czy przegrana
    double m_game_over_position; //pozycja napisu "GAME OVER"
    bool m_finished; //czy koniec levelu (przegrana lub przejście)
    bool m_pause;

    int m_enemy_respown_position;
};

#endif // GAME_H
