#ifndef LEVEL_ENVIRONMENT_H
#define LEVEL_ENVIRONMENT_H

#include "../../objects/object.h"
#include "../../objects/eagle.h"
#include "../../objects/tank.h"
#include "../../objects/bullet.h"
#include <vector>


class LevelEnvironment
{
public:
    LevelEnvironment(int current_level);
    ~LevelEnvironment();

    void drawFirstLayer(Renderer &renderer);
    void drawSecondLayer(Renderer &renderer);

    void update(Uint32 dt);

    void destroyEagle();
    void startEagleProtection();

    void checkCollisionTankWithLevel(Tank *tank, Uint32 dt);
    void checkCollisionBulletWithLevel(Bullet *bullet);
    bool checkCollisionBulletWithEagle(Bullet *bullet);
    bool checkCollisionWithEagle(Rect &rect);

    Point getEagleCenter() const;
private:
    void loadLevel();
    void changeEagleSurroundingsToStoneWalls();
    void changeEagleSurroundingsToBrickWalls();

    int m_current_level;

    std::vector<std::vector<Object *>> m_tile_objects;

    Rect m_eagle_tile_rect;
    Eagle *m_eagle;
    bool m_eagle_protected;
    Uint32 m_eagle_protection_time;


    Rect m_map_outside_colision_top_rect;
    Rect m_map_outside_colision_bottom_rect;
    Rect m_map_outside_colision_left_rect;
    Rect m_map_outside_colision_right_rect;
};


#endif