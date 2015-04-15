#ifndef GAME_H
#define GAME_H
#include "appstate.h"
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/enemy.h"
#include "../objects/bullet.h"
#include "../objects/brick.h"
#include <vector>
#include <string>

class Game : public AppState
{
public:
    Game();
    ~Game();
    void draw();
    void update(Uint32 dt);
    void eventProces(SDL_Event* ev);
    void loadLevel(std::string path);

private:
    void clearLevel();

    void checkCollisionTankWithLevel(Tank* tank, Uint32 dt);
    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt);
    void checkCollisionBulletWithLevel(Bullet* bullet);
    void checkCollisionBulletWithTanks(Bullet* bullet, Tank* tank);
    void checkCollisionTwoBullets(Bullet* bullet1, Bullet* bullet2);

    void nextLevel();

    int m_level_horizontal_size;
    int m_level_vertical_size;
    std::vector< std::vector <Object*> > m_level; //level bez krzaków
    std::vector<Object*> m_bushes;

    std::vector<Enemy*> m_enemies;
    std::vector<Player*> m_players;

    std::vector<SDL_Rect*> m_rec;

    unsigned m_current_level;
};

#endif // GAME_H
