#include "game.h"
#include "../engine/engine.h"
#include "../objects/objectfactory.h"
#include "../appconfig.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <algorithm>


Game::Game()
{
    m_level_horizontal_size = 0;
    m_level_vertical_size = 0;
    m_current_level = 0;

    nextLevel();
}

Game::~Game()
{
    clearLevel();
}

void Game::draw()
{
    Engine& engine = Engine::getEngine();
    //SpriteConfig* sprite_config = engine.getSpriteConfig();
    Renderer* renderer = engine.getRenderer();
    renderer->clear();

    for(auto row : m_level)
        for(auto item : row)
            if(item != nullptr) item->draw();

    for(auto enemy : m_enemies) enemy->draw();
    for(auto player : m_players) player->draw();

    for(auto r : m_rec)
        renderer->drawRect(r, {0, 0, 255});

    for(auto bush : m_bushes)
        if(bush != nullptr) bush->draw();

    renderer->drawText({100, 100}, std::string("dupa"), {200, 255, 0, 255});
    renderer->flush();
}

void Game::update(Uint32 dt)
{
    if(dt > 40) return;

    std::vector<Player*>::iterator pl1, pl2;
    std::vector<Enemy*>::iterator en1, en2;

    //sprawdzenie kolizji czołgów graczy ze sobą
    for(pl1 = m_players.begin(); pl1 != m_players.end(); pl1++)
        for(pl2 = pl1 + 1; pl2 != m_players.end(); pl2++)
            checkCollisionTwoTanks(*pl1, *pl2, dt);

    //sprawdzenie kolizji czołgów przeciwników ze sobą
    for(en1 = m_enemies.begin(); en1 != m_enemies.end(); en1++)
         for(en2 = en1 + 1; en2 != m_enemies.end(); en2++)
            checkCollisionTwoTanks(*en1, *en2, dt);

    //sprawdzenie kolizji kuli z lewelem
    for(auto enemy : m_enemies) checkCollisionBulletWithLevel(enemy->bullet);
    for(auto player : m_players) checkCollisionBulletWithLevel(player->bullet);


    for(auto player : m_players)
        for(auto enemy : m_enemies)
        {
            //sprawdzenie kolizji czołgów przeciwników z graczami
            checkCollisionTwoTanks(player, enemy, dt);
            //sprawdzenie kolizji pocisku gracza z przeciwnikiem
            checkCollisionBulletWithTanks(player->bullet, enemy);
            //sprawdzenie kolizji pocisku gracza z pociskiem przeciwnika
            checkCollisionTwoBullets(player->bullet, enemy->bullet);
        }

    //sprawdzenie kolizji pocisku przeciknika z graczem
    for(auto enemy : m_enemies)
        for(auto player : m_players)
            checkCollisionBulletWithTanks(enemy->bullet, player);

    //Sprawdzenie kolizji czołgów z poziomem
    for(auto enemy : m_enemies) checkCollisionTankWithLevel(enemy, dt);
    for(auto player : m_players) checkCollisionTankWithLevel(player, dt);


    //Update wszystkich obiektów
    for(auto enemy : m_enemies) enemy->update(dt);
    for(auto player : m_players) player->update(dt);

    for(auto row : m_level)
        for(auto item : row)
            if(item != nullptr) item->update(dt);

    for(auto bush : m_bushes)
        if(bush != nullptr) bush->update(dt);

    //usunięcie niepotrzebnych czołgów
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](Enemy*e){if(e->to_erase) {delete e; return true;} return false;}), m_enemies.end());
    m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [](Player*p){if(p->to_erase) {delete p; return true;} return false;}), m_players.end());
}

void Game::eventProces(SDL_Event *ev)
{
    switch(ev->type)
    {
    case SDL_KEYDOWN:
        switch(ev->key.keysym.sym)
        {
        case SDLK_UP:
            m_players.at(0)->speed = Tank::default_speed;
            m_players.at(0)->setDirection(D_UP);
            break;
        case SDLK_DOWN:
            m_players.at(0)->speed = Tank::default_speed;
            m_players.at(0)->setDirection(D_DOWN);
            break;
        case SDLK_RIGHT:
            m_players.at(0)->speed = Tank::default_speed;
            m_players.at(0)->setDirection(D_RIGHT);
            break;
        case SDLK_LEFT:
            m_players.at(0)->speed = Tank::default_speed;
            m_players.at(0)->setDirection(D_LEFT);
            break;
        case SDLK_SPACE:
            m_players.at(0)->fire();
            break;
        case SDLK_r:
            m_players.at(0)->respawn();
            break;
        case SDLK_n:
            nextLevel();
            break;
        case SDLK_1:
            m_enemies.push_back(new Enemy(1, 1, ST_TANK_A));
            break;
        case SDLK_2:
            m_enemies.push_back(new Enemy(50, 1, ST_TANK_B));
            break;
        case SDLK_3:
            m_enemies.push_back(new Enemy(100, 1, ST_TANK_C));
            break;
        case SDLK_4:
            m_enemies.push_back(new Enemy(150, 1, ST_TANK_D));
            break;
        }
        break;
    }
}

/*
. = puste pole
# = murek
@ = kamień
% = krzaki
~ = woda
- = lód
 */

void Game::loadLevel(std::string path)
{
    clearLevel();
    std::fstream level(path, std::ios::in);
    std::string line;
    int j = -1;

    if(level.is_open())
    {
        while(!level.eof())
        {
            std::getline(level, line);
            std::vector<Object*> row;
            j++;
            for(int i = 0; i < line.size(); i++)
            {
                Object* obj;
                switch(line.at(i))
                {
                case '#' : obj = ObjectFactory::Create(i * AppConfig::tile_width, j * AppConfig::tile_height, ST_BRICK_WALL); break;
                case '@' : obj = ObjectFactory::Create(i * AppConfig::tile_width, j * AppConfig::tile_height, ST_STONE_WALL); break;
                case '%' : m_bushes.push_back(ObjectFactory::Create(i * AppConfig::tile_width, j * AppConfig::tile_height, ST_BUSH)); obj =  nullptr; break;
                case '~' : obj = ObjectFactory::Create(i * AppConfig::tile_width, j * AppConfig::tile_height, ST_WATER); break;
                case '-' : obj = ObjectFactory::Create(i * AppConfig::tile_width, j * AppConfig::tile_height, ST_ICE); break;
                default: obj = nullptr;
                }
                row.push_back(obj);
            }
            m_level.push_back(row);
        }
    }

    m_level_vertical_size = m_level.size();
    if(m_level_vertical_size)
        m_level_horizontal_size = m_level.at(0).size();
    else m_level_horizontal_size = 0;
}

void Game::clearLevel()
{
    for(auto enemy : m_enemies) delete enemy;
    m_enemies.clear();

    for(auto player : m_players) delete player;
    m_players.clear();

    for(auto row : m_level)
    {
        for(auto item : row) if(item != nullptr) delete item;
        row.clear();
    }
    m_level.clear();

    for(auto bush : m_bushes) if(bush != nullptr) delete bush;
    m_bushes.clear();
}

void Game::checkCollisionTankWithLevel(Tank* tank, Uint32 dt)
{
    for(auto r : m_rec) delete r;
    m_rec.clear();

    int row_start, row_end;
    int column_start, column_end;

    SDL_Rect pr, *lr;
    Object* o;

    //========================kolizja z elementami mapy========================
    switch(tank->direction)
    {
    case D_UP:
        row_end = tank->collision_rect.y / AppConfig::tile_height;
        row_start = row_end - 1;
        column_start = tank->collision_rect.x / AppConfig::tile_width - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_width + 1;
        break;
    case D_RIGHT:
        column_start = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_width;
        column_end = column_start + 1;
        row_start = tank->collision_rect.y / AppConfig::tile_height - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_height + 1;
        break;
    case D_DOWN:
        row_start = (tank->collision_rect.y + tank->collision_rect.h)/ AppConfig::tile_height;
        row_end = row_start + 1;
        column_start = tank->collision_rect.x / AppConfig::tile_width - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_width + 1;
        break;
    case D_LEFT:
        column_end = tank->collision_rect.x / AppConfig::tile_width;
        column_start = column_end - 1;
        row_start = tank->collision_rect.y / AppConfig::tile_height - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_height + 1;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= m_level_horizontal_size) column_end = m_level_horizontal_size - 1;
    if(row_end >= m_level_vertical_size) row_end = m_level_vertical_size - 1;

    pr = tank->nextCollisionRect(dt);
    SDL_Rect intersect_rect;

    for(int i = row_start; i <= row_end; i++)
        for(int j = column_start; j <= column_end ;j++)
        {
            if(tank->stop) break;
            o = m_level.at(i).at(j);
            if(o == nullptr) continue;
            if(o->type == ST_ICE) continue;

            lr = &o->collision_rect;

            intersect_rect = intersectRect(lr, &pr);
            if(intersect_rect.w > 0 && intersect_rect.h > 0)
            {
                tank->collide(intersect_rect);
                break;
            }
        }

    //========================kolizja z granicami mapy========================
    SDL_Rect outside_map_rect;
    //prostokąt po lewej stronie mapy
    outside_map_rect.x = -AppConfig::tile_width;
    outside_map_rect.y = -AppConfig::tile_height;
    outside_map_rect.w = AppConfig::tile_width;
    outside_map_rect.h = AppConfig::map_height + 2 * AppConfig::tile_height;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po prawej stronie mapy
    outside_map_rect.x = AppConfig::map_width;
    outside_map_rect.y = -AppConfig::tile_height;
    outside_map_rect.w = AppConfig::tile_width;
    outside_map_rect.h = AppConfig::map_height + 2 * AppConfig::tile_height;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po górnej stronie mapy
    outside_map_rect.x = 0;
    outside_map_rect.y = -AppConfig::tile_height;
    outside_map_rect.w = AppConfig::map_width;
    outside_map_rect.h = AppConfig::tile_height;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po dolnej stronie mapy
    outside_map_rect.x = 0;
    outside_map_rect.y = AppConfig::map_height;
    outside_map_rect.w = AppConfig::map_width;
    outside_map_rect.h = AppConfig::tile_height;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);
}

void Game::checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt)
{
    SDL_Rect cr1 = tank1->nextCollisionRect(dt);
    SDL_Rect cr2 = tank2->nextCollisionRect(dt);
    SDL_Rect intersect_rect = intersectRect(&cr1, &cr2);

    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        tank1->collide(intersect_rect);
        tank2->collide(intersect_rect);
    }
}

void Game::checkCollisionBulletWithLevel(Bullet* bullet)
{
    if(bullet == nullptr) return;
    if(bullet->colide) return;

    int row_start, row_end;
    int column_start, column_end;

    SDL_Rect* br, *lr;
    SDL_Rect intersect_rect;
    Object* o;

    //========================kolizja z elementami mapy========================
    switch(bullet->direction)
    {
    case D_UP:
        row_start = row_end = bullet->collision_rect.y / AppConfig::tile_height;
        column_start = bullet->collision_rect.x / AppConfig::tile_width;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_width;
        break;
    case D_RIGHT:
        column_start = column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_width;
        row_start = bullet->collision_rect.y / AppConfig::tile_height;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_height;
        break;
    case D_DOWN:
        row_start = row_end = (bullet->collision_rect.y + bullet->collision_rect.h)/ AppConfig::tile_height;
        column_start = bullet->collision_rect.x / AppConfig::tile_width;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_width;
        break;
    case D_LEFT:
        column_start = column_end = bullet->collision_rect.x / AppConfig::tile_width;
        row_start = bullet->collision_rect.y / AppConfig::tile_height;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_height;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= m_level_horizontal_size) column_end = m_level_horizontal_size - 1;
    if(row_end >= m_level_vertical_size) row_end = m_level_vertical_size - 1;

    br = &bullet->collision_rect;

    for(int i = row_start; i <= row_end; i++)
        for(int j = column_start; j <= column_end; j++)
        {
            o = m_level.at(i).at(j);
            if(o == nullptr) continue;
            if(o->type == ST_ICE || o->type == ST_WATER) continue;

            lr = &o->collision_rect;
            intersect_rect = intersectRect(lr, br);

            if(intersect_rect.w > 0 && intersect_rect.h > 0)
            {
                if(bullet->increased_damage)
                {
                    delete o;
                    m_level.at(i).at(j) = nullptr;
                }
                else if(o->type == ST_BRICK_WALL)
                {
                    Brick* brick = dynamic_cast<Brick*>(o);
                    brick->bulletHit(bullet->direction);
                    if(brick->to_erase)
                    {
                        delete brick;
                        m_level.at(i).at(j) = nullptr;
                    }
                }

                bullet->destroy();
            }
        }

    //========================kolizja z granicami mapy========================
    if(br->x < 0 || br->y < 0 || br->x + br->w > AppConfig::map_width || br->y + br->h > AppConfig::map_height)
    {
        bullet->destroy();
    }
}

void Game::checkCollisionBulletWithTanks(Bullet *bullet, Tank *tank)
{
    if(bullet == nullptr) return;
    if(bullet->colide) return;

    SDL_Rect intersect_rect = intersectRect(&bullet->collision_rect, &tank->collision_rect);

    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        bullet->destroy();
        tank->destroy();
    }
}

void Game::checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2)
{
    if(bullet1 == nullptr || bullet2 == nullptr) return;
    if(bullet1->to_erase || bullet2->to_erase) return;

    SDL_Rect intersect_rect = intersectRect(&bullet1->collision_rect, &bullet2->collision_rect);

    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        bullet1->destroy();
        bullet2->destroy();
    }
}

void Game::nextLevel()
{
    m_current_level++;

    auto my_itoa = [] (int value)
    {
        std::string buf;
        for(; value; value /= 10) buf = "0123456789abcdef"[value % 10] + buf;
        return buf;
    };
    std::string level_path = AppConfig::levels_path + my_itoa(m_current_level);

    loadLevel(level_path);

    m_players.push_back(new Player(AppConfig::player1_starting_point_x, AppConfig::player1_starting_point_y, ST_PLAYER_2));

    m_enemies.push_back(new Enemy(1, 1, ST_TANK_A));
    m_enemies.push_back(new Enemy(50, 1, ST_TANK_B));
    m_enemies.push_back(new Enemy(100, 1, ST_TANK_C));
    m_enemies.push_back(new Enemy(150, 1, ST_TANK_D));
}
