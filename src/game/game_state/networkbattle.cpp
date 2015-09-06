#include "networkbattle.h"

#include "../engine/engine.h"
#include "../../appconfig.h"
#include "menu.h"
#include "scores.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iostream>

NetworkBattle::NetworkBattle(Game *parent) : GameState(parent)
{
    key_num = 0;

    m_level_columns_count = 0;
    m_level_rows_count = 0;
    m_current_level = 0;
    m_eagle = nullptr;
    m_player_count = 1;
    m_enemy_redy_time = 0;
    m_pause = false;
    m_level_end_time = 0;
    m_protect_eagle = false;
    m_protect_eagle_time = 0;
    m_enemy_respown_position = 0;

    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);

    nextLevel();
}

NetworkBattle::NetworkBattle(Game* parent, int players_count) : GameState(parent)
{
    key_num = 0;

    m_level_columns_count = 0;
    m_level_rows_count = 0;
    m_current_level = 0;
    m_eagle = nullptr;
    m_player_count = players_count;
    m_pause = false;
    m_level_end_time = 0;
    m_protect_eagle = false;
    m_protect_eagle_time = 0;
    m_enemy_respown_position = 0;

    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);

    nextLevel();
}

NetworkBattle::NetworkBattle(Game *parent, std::vector<Player *> players, int previous_level) : GameState(parent)
{
    key_num = 0;

    m_level_columns_count = 0;
    m_level_rows_count = 0;
    m_current_level = previous_level;
    m_eagle = nullptr;
    m_players = players;
    m_player_count = m_players.size();
    for(auto player : m_players)
    {
        player->clearFlag(TSF_MENU);
        player->lives_count++;
        player->respawn();
    }
    m_pause = false;
    m_level_end_time = 0;
    m_protect_eagle = false;
    m_protect_eagle_time = 0;
    m_enemy_respown_position = 0;

    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);

    nextLevel();
}

NetworkBattle::~NetworkBattle()
{
    clearLevel();
}

void NetworkBattle::draw()
{
    Engine& engine = Engine::getEngine();
    Renderer* renderer = engine.getRenderer();
    renderer->clear();

    if(m_level_start_screen)
    {
        std::string level_name = "STAGE " + Engine::intToString(m_current_level);
        renderer->drawText(nullptr, level_name, {255, 255, 255, 255}, 1);
    }
    else
    {
        renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 0}, true);
        for(auto row : m_level)
            for(auto item : row)
                if(item != nullptr) item->draw();

        for(auto player : m_players) player->draw();
        for(auto enemy : m_enemies) enemy->draw();
        for(auto bush : m_bushes) bush->draw();
        for(auto bonus : m_bonuses) bonus->draw();
        m_eagle->draw();

        if(m_game_over)
        {
            SDL_Point pos;
            pos.x = -1;
            pos.y = m_game_over_position;
            renderer->drawText(&pos, AppConfig::game_over_text, {255, 10, 10, 255});
        }

        //===========Status gry===========
        SDL_Rect src = engine.getSpriteConfig()->getSpriteData(ST_LEFT_ENEMY)->rect;
        SDL_Rect dst;
        SDL_Point p_dst;
        //wrogowie do zabicia
        for(int i = 0; i < m_enemy_to_kill; i++)
        {
            dst = {AppConfig::status_rect.x + 8 + src.w * (i % 2), 5 + src.h * (i / 2), src.w, src.h};
            renderer->drawObject(&src, &dst);
        }
        //życia graczy
        int i = 0;
        for(auto player : m_players)
        {
            dst = {AppConfig::status_rect.x + 5, i * 18 + 180, 16, 16};
            p_dst = {dst.x + dst.w + 2, dst.y + 3};
            i++;
            renderer->drawObject(&player->src_rect, &dst);
            renderer->drawText(&p_dst, Engine::intToString(player->lives_count), {0, 0, 0, 255}, 3);
        }
        //numer mapy
        src = engine.getSpriteConfig()->getSpriteData(ST_STAGE_STATUS)->rect;
        dst = {AppConfig::status_rect.x + 8, 185 + (m_players.size() + m_killed_players.size()) * 18, src.w, src.h};
        p_dst = {dst.x + 10, dst.y + 26};
        renderer->drawObject(&src, &dst);
        renderer->drawText(&p_dst, Engine::intToString(m_current_level), {0, 0, 0, 255}, 2);

        if(m_pause)
            renderer->drawText(nullptr, std::string("PAUSE"), {200, 0, 0, 255}, 1);
    }

    renderer->flush();
}

void NetworkBattle::update(Uint32 dt)
{
    if(dt > 40) return;

    if(m_level_start_screen)
    {
        if(m_level_start_time > AppConfig::level_start_time)
            m_level_start_screen = false;

        m_level_start_time += dt;
    }
    else
    {
        if(m_pause) return;  // TODO z konfiga

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
        for(auto enemy : m_enemies)
            for(auto bullet : enemy->bullets)
                checkCollisionBulletWithLevel(bullet);
        for(auto player : m_players)
            for(auto bullet : player->bullets)
            {
                checkCollisionBulletWithLevel(bullet);
                checkCollisionBulletWithBush(bullet);
            }


        for(auto player : m_players)
            for(auto enemy : m_enemies)
            {
                //sprawdzenie kolizji czołgów przeciwników z graczami
                checkCollisionTwoTanks(player, enemy, dt);
                //sprawdzenie kolizji pocisków gracza z przeciwnikiem
                checkCollisionPlayerBulletsWithEnemy(player, enemy);

                //sprawdzenie kolizji pocisku gracza z pociskiem przeciwnika
                for(auto bullet1 : player->bullets)
                     for(auto bullet2 : enemy->bullets)
                            checkCollisionTwoBullets(bullet1, bullet2);
            }

        //sprawdzenie kolizji pocisku przeciknika z graczem
        for(auto enemy : m_enemies)
            for(auto player : m_players)
                    checkCollisionEnemyBulletsWithPlayer(enemy, player);

        //sprawdzanie kolizji gracza z bunusem
        for(auto player : m_players)
            for(auto bonus : m_bonuses)
                checkCollisionPlayerWithBonus(player, bonus);

        //Sprawdzenie kolizji czołgów z poziomem
        for(auto enemy : m_enemies) checkCollisionTankWithLevel(enemy, dt);
        for(auto player : m_players) checkCollisionTankWithLevel(player, dt);

        //nadanie celów przeciwników
        int min_metric; // 2 * 26 * 16
        int metric;
        SDL_Point target;
        for(auto enemy : m_enemies)
        {
            min_metric = 832;
            if(enemy->type == ST_TANK_A || enemy->type == ST_TANK_D)
                for(auto player : m_players)
                {
                    metric = fabs(player->dest_rect.x - enemy->dest_rect.x) + fabs(player->dest_rect.y - enemy->dest_rect.y);
                    if(metric < min_metric)
                    {
                        min_metric = metric;
                        target = {player->dest_rect.x + player->dest_rect.w / 2, player->dest_rect.y + player->dest_rect.h / 2};
                    }
                }
            metric = fabs(m_eagle->dest_rect.x - enemy->dest_rect.x) + fabs(m_eagle->dest_rect.y - enemy->dest_rect.y);
            if(metric < min_metric)
            {
                min_metric = metric;
                target = {m_eagle->dest_rect.x + m_eagle->dest_rect.w / 2, m_eagle->dest_rect.y + m_eagle->dest_rect.h / 2};
            }

            enemy->target_position = target;
        }

        //Update wszystkich obiektów
        for(auto enemy : m_enemies) enemy->update(dt);
        for(auto player : m_players) player->update(dt);
        for(auto bonus : m_bonuses) bonus->update(dt);
        m_eagle->update(dt);

        for(auto row : m_level)
            for(auto item : row)
                if(item != nullptr) item->update(dt);


        for(auto bush : m_bushes) bush->update(dt);

        //usunięcie niepotrzebnych elementów
        m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](Enemy*e){if(e->to_erase) {delete e; return true;} return false;}), m_enemies.end());
        m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [this](Player*p){if(p->to_erase) {m_killed_players.push_back(p); return true;} return false;}), m_players.end());
        m_bonuses.erase(std::remove_if(m_bonuses.begin(), m_bonuses.end(), [](Bonus*b){if(b->to_erase) {delete b; return true;} return false;}), m_bonuses.end());
        m_bushes.erase(std::remove_if(m_bushes.begin(), m_bushes.end(), [](Object*b){if(b->to_erase) {delete b; return true;} return false;}), m_bushes.end());

        //dodanie nowego przeciwnika
        m_enemy_redy_time += dt;
        if(m_enemies.size() < (AppConfig::enemy_max_count_on_map < m_enemy_to_kill ? AppConfig::enemy_max_count_on_map : m_enemy_to_kill) && m_enemy_redy_time > AppConfig::enemy_redy_time)
        {
            m_enemy_redy_time = 0;
            generateEnemy();
        }

        if(m_enemies.empty() && m_enemy_to_kill <= 0)
        {
            m_level_end_time += dt;
            if(m_level_end_time > AppConfig::level_end_time)
                m_finished = true;
        }

        if(m_players.empty() && !m_game_over)
        {
            m_eagle->destroy();
            m_game_over_position = AppConfig::map_rect.h;
            m_game_over = true;
        }

        if(m_game_over)
        {
            if(m_game_over_position < 10) m_finished = true;
            else m_game_over_position -= AppConfig::game_over_entry_speed * dt;
        }

        if(m_protect_eagle)
        {
            m_protect_eagle_time += dt;
            if(m_protect_eagle_time > AppConfig::protect_eagle_time)
            {
                m_protect_eagle = false;
                m_protect_eagle_time = 0;
                for(int i = 0; i < 3; i++)
                {
                    if(m_level.at(m_level_rows_count - i - 1).at(11) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(11);
                    m_level.at(m_level_rows_count - i - 1).at(11) = new Brick(11 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1) * AppConfig::tile_rect.h);

                    if(m_level.at(m_level_rows_count - i - 1).at(14) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(14);
                    m_level.at(m_level_rows_count - i - 1).at(14) = new Brick(14 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1)  * AppConfig::tile_rect.h);
                }
                for(int i = 12; i < 14; i++)
                {
                    if(m_level.at(m_level_rows_count - 3).at(i) != nullptr)
                        delete m_level.at(m_level_rows_count - 3).at(i);
                    m_level.at(m_level_rows_count - 3).at(i) = new Brick(i * AppConfig::tile_rect.w, (m_level_rows_count - 3) * AppConfig::tile_rect.h);
                }
            }

            if(m_protect_eagle && m_protect_eagle_time > AppConfig::protect_eagle_time / 4 * 3 && m_protect_eagle_time / AppConfig::bonus_blink_time % 2)
            {
                for(int i = 0; i < 3; i++)
                {
                    if(m_level.at(m_level_rows_count - i - 1).at(11) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(11);
                    m_level.at(m_level_rows_count - i - 1).at(11) = new Brick(11 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1) * AppConfig::tile_rect.h);

                    if(m_level.at(m_level_rows_count - i - 1).at(14) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(14);
                    m_level.at(m_level_rows_count - i - 1).at(14) = new Brick(14 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1)  * AppConfig::tile_rect.h);
                }
                for(int i = 12; i < 14; i++)
                {
                    if(m_level.at(m_level_rows_count - 3).at(i) != nullptr)
                        delete m_level.at(m_level_rows_count - 3).at(i);
                    m_level.at(m_level_rows_count - 3).at(i) = new Brick(i * AppConfig::tile_rect.w, (m_level_rows_count - 3) * AppConfig::tile_rect.h);
                }
            }
            else if(m_protect_eagle)
            {
                for(int i = 0; i < 3; i++)
                {
                    if(m_level.at(m_level_rows_count - i - 1).at(11) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(11);
                    m_level.at(m_level_rows_count - i - 1).at(11) = new Object(11 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1) * AppConfig::tile_rect.h, ST_STONE_WALL);

                    if(m_level.at(m_level_rows_count - i - 1).at(14) != nullptr)
                        delete m_level.at(m_level_rows_count - i - 1).at(14);
                    m_level.at(m_level_rows_count - i - 1).at(14) = new Object(14 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1)  * AppConfig::tile_rect.h, ST_STONE_WALL);
                }
                for(int i = 12; i < 14; i++)
                {
                    if(m_level.at(m_level_rows_count - 3).at(i) != nullptr)
                        delete m_level.at(m_level_rows_count - 3).at(i);
                    m_level.at(m_level_rows_count - 3).at(i) = new Object(i * AppConfig::tile_rect.w, (m_level_rows_count - 3) * AppConfig::tile_rect.h, ST_STONE_WALL);
                }
            }
        }
    }

    NetworkState state;
    EnterCriticalSection(parent->critical_section);
        state = parent->shared_data->network_state;
    LeaveCriticalSection(parent->critical_section);

    if(state == NetworkState::SERVER)
    {
//        createSeeds();
    }
}

void NetworkBattle::eventProcess()
{
//    std::cout << "eventProcess 1" << std::endl;
    int event_count = 0;
    SOCKET player_socket;
    bool is_empty = true;
    Event* e = nullptr;
    EnterCriticalSection(parent->critical_section);
        is_empty = parent->shared_data->received_events_queue.empty();
    LeaveCriticalSection(parent->critical_section);
    while(!is_empty)
    {
        event_count++;
        EnterCriticalSection(parent->critical_section);
            e = parent->shared_data->received_events_queue.pop();
        LeaveCriticalSection(parent->critical_section);
        switch (e->type)
        {
        case EventType::GENERATE_EVENT_TYPE:
        {
            GenerateEvent* gen = (GenerateEvent*)e;
            random_seed = gen->seed1.l_value;
            srand(random_seed);
            break;
        }
        case EventType::PLAYER_ID_TYPE:
        {
            PlayerNameEvent* p = (PlayerNameEvent*)e;
            player_socket = p->player_id.l_value;
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->player_name[player_socket] = std::string(p->name);
            LeaveCriticalSection(parent->critical_section);
            break;
        }
        case EventType::DISCONNECT_EVENT_TYPE:
        {
            DisconnectEvent* d = (DisconnectEvent*)e;
            player_socket = d->player_id.l_value;
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->player_name.erase(player_socket);
            LeaveCriticalSection(parent->critical_section);
            break;
        }
        case EventType::KEY_EVENT_TYPE:
        {
            key_num++;
//            cout << "KEY_EVENT_TYPE frame: " << parent->getCurrentFrame() << " Key num: " << key_num << std::endl;
            KeyEvent* key =(KeyEvent*)e;
//            cout << "Player1 curr: " << parent->getCurrentFrame()  << " posX: " << m_players.at(0)->pos_x << " posY: " << m_players.at(0)->pos_y << endl;
            switch(key->key_type)
            {
            case KeyEvent::KeyType::UP:
            {

                for(Player* p : m_players)
                {
                    if(p->object_id == key->id_tank.l_value)
                    {
                        checkCollisionTankWithLevel(p, AppConfig::game_speed);
                        p->move_next = true;
//                        p->next_direction = D_UP;
                        p->setDirection(D_UP);
                        p->speed = p->default_speed;
                        p->move();
                        break;
                    }
                }
                break;
            }
            case KeyEvent::KeyType::DOWN:
            {
                for(Player* p : m_players)
                {
                    if(p->object_id == key->id_tank.l_value)
                    {
                        checkCollisionTankWithLevel(p, AppConfig::game_speed);
                        p->move_next = true;
//                        p->next_direction = D_DOWN;
                        p->setDirection(D_DOWN);
                        p->speed = p->default_speed;
                        p->move();
                        break;
                    }
                }
                break;
            }
            case KeyEvent::KeyType::LEFT:
            {
                for(Player* p : m_players)
                {
                    if(p->object_id == key->id_tank.l_value)
                    {
                        checkCollisionTankWithLevel(p, AppConfig::game_speed);
                        p->move_next = true;
//                        p->next_direction = D_LEFT;
                        p->setDirection(D_LEFT);
                        p->speed = p->default_speed;
                        p->move();
                        break;
                    }
                }
                break;
            }
            case KeyEvent::KeyType::RIGHT:
            {
                for(Player* p : m_players)
                {
                    if(p->object_id == key->id_tank.l_value)
                    {
                        checkCollisionTankWithLevel(p, AppConfig::game_speed);
                        p->move_next = true;
//                        p->next_direction = D_RIGHT;
                        p->setDirection(D_RIGHT);
                        p->speed = p->default_speed;
                        p->move();
                        break;
                    }
                }
                break;
            }
            case KeyEvent::KeyType::FIRE:
            {
                for(Player* p : m_players)
                {
                    if(p->object_id == key->id_tank.l_value)
                    {
                        p->fire();
                        break;
                    }
                }
                break;
            }
            case KeyEvent::KeyType::PAUSE:
            {
                m_pause = !m_pause;
                break;
            }
            }
            break;
        }
        case EventType::POSITION_TYPE:
        {
            PositionEvent* pos = (PositionEvent*)e;
            switch(pos->obj)
            {
                case PositionEvent::PosObj::TANK:
                {
                    bool found = false;
                    for(Player* p : m_players)
                    {
                        if(p->object_id == pos->obj_id.l_value)
                        {
                            p->pos_x = pos->pos_x.d_value;
                            p->pos_y = pos->pos_y.d_value;
                            found = true;
                            break;
                        }
                    }

                    if(!found)
                    {
                        for(Enemy* e : m_enemies)
                        {
                            if(e->object_id == pos->obj_id.l_value)
                            {
                                e->pos_x = pos->pos_x.d_value;
                                e->pos_y = pos->pos_y.d_value;
                                found = true;
                                break;
                            }
                        }
                    }
                    if(!found)
                        std::cout << "Pos type NOT found" << std::endl;

                    break;
                }
            default:
                std::cout << "Unknown POSITION_TYPE " << (int) pos->obj << std::endl;
            }
            break;
        }
        case EventType::SPEED_CHANGE_TYPE:
        {
            break;
        }
        default:
            cout << "Nieznany event" << std::endl;
            break;
        }

        delete e;
        EnterCriticalSection(parent->critical_section);
            is_empty = parent->shared_data->received_events_queue.empty();
        LeaveCriticalSection(parent->critical_section);
    }
//    std::cout << "eventProcess 2" << std::endl;
//    if(event_count)
//        std::cout << "event_count: " << (int)event_count << std::endl;
}

void NetworkBattle::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        switch(ev->key.keysym.sym)
        {
//        case SDLK_n:
//            m_enemy_to_kill = 0;
//            m_finished = true;
//            break;
//        case SDLK_b:
//            m_enemy_to_kill = 0;
//            m_current_level -= 2;
//            m_finished = true;
//            break;
        case SDLK_t:
        {
            AppConfig::show_enemy_target = !AppConfig::show_enemy_target;
            break;
        }
        case SDLK_RETURN:
        {
            KeyEvent* pause = new KeyEvent;
            pause->key_type = KeyEvent::KeyType::PAUSE;
//            pause->frame_number.l_value = parent->getCurrentFrame() + pause->priority;
            pause->id_tank.l_value = 0;

            EnterCriticalSection(parent->critical_section);
                parent->shared_data->transmit_events.addEvent(pause);
            LeaveCriticalSection(parent->critical_section);

            break;
        }
        case SDLK_ESCAPE:
        {
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->network_state = NetworkState::NONE;
            LeaveCriticalSection(parent->critical_section);
            m_finished = true;
            break;
        }
        }
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

void NetworkBattle::loadLevel(std::string path)
{
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
            for(unsigned i = 0; i < line.size(); i++)
            {
                Object* obj;
                switch(line.at(i))
                {
                case '#' : obj = new Brick(i * AppConfig::tile_rect.w, j * AppConfig::tile_rect.h); break;
                case '@' : obj = new Object(i * AppConfig::tile_rect.w, j * AppConfig::tile_rect.h, ST_STONE_WALL); break;
                case '%' : m_bushes.push_back(new Object(i * AppConfig::tile_rect.w, j * AppConfig::tile_rect.h, ST_BUSH)); obj =  nullptr; break;
                case '~' : obj = new Object(i * AppConfig::tile_rect.w, j * AppConfig::tile_rect.h, ST_WATER); break;
                case '-' : obj = new Object(i * AppConfig::tile_rect.w, j * AppConfig::tile_rect.h, ST_ICE); break;
                default: obj = nullptr;
                }
                row.push_back(obj);
            }
            m_level.push_back(row);
        }
    }

    m_level_rows_count = m_level.size();
    if(m_level_rows_count)
        m_level_columns_count = m_level.at(0).size();
    else m_level_columns_count = 0;

    //tworzymy orzełka
    m_eagle = new Eagle(12 * AppConfig::tile_rect.w, (m_level_rows_count - 2) * AppConfig::tile_rect.h);

    //wyczyszczenie miejsca orzełeka
    for(int i = 12; i < 14 && i < m_level_columns_count; i++)
    {
        for(int j = m_level_rows_count - 2; j < m_level_rows_count; j++)
        {
            if(m_level.at(j).at(i) != nullptr)
            {
                delete m_level.at(j).at(i);
                m_level.at(j).at(i) = nullptr;
            }
        }
    }
}

bool NetworkBattle::finished() const
{
    return m_finished;
}

GameState* NetworkBattle::nextState()
{
    if(m_game_over || m_enemy_to_kill <= 0)
    {
        m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [this](Player*p){m_killed_players.push_back(p); return true;}), m_players.end());
        Scores* scores = new Scores(parent, m_killed_players, m_current_level, m_game_over);
        return scores;
    }
    Menu* m = new Menu(parent);
    return m;
}

void NetworkBattle::clearLevel()
{
    for(auto enemy : m_enemies) delete enemy;
    m_enemies.clear();

    for(auto player : m_players) delete player;
    m_players.clear();

    for(auto bonus : m_bonuses) delete bonus;
    m_bonuses.clear();

    for(auto row : m_level)
    {
        for(auto item : row) if(item != nullptr) delete item;
        row.clear();
    }
    m_level.clear();

    for(auto bush : m_bushes)  delete bush;
    m_bushes.clear();

    if(m_eagle != nullptr) delete m_eagle;
    m_eagle = nullptr;
}

void NetworkBattle::checkCollisionTankWithLevel(Tank* tank, Uint32 dt)
{
    if(tank->to_erase) return;

    int row_start, row_end;
    int column_start, column_end;

    SDL_Rect pr, *lr;
    Object* o;

    //========================kolizja z elementami mapy========================
    switch(tank->direction)
    {
    case D_UP:
        row_end = tank->collision_rect.y / AppConfig::tile_rect.h;
        row_start = row_end - 1;
        column_start = tank->collision_rect.x / AppConfig::tile_rect.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_rect.w + 1;
        break;
    case D_RIGHT:
        column_start = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_rect.w;
        column_end = column_start + 1;
        row_start = tank->collision_rect.y / AppConfig::tile_rect.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_rect.h + 1;
        break;
    case D_DOWN:
        row_start = (tank->collision_rect.y + tank->collision_rect.h)/ AppConfig::tile_rect.h;
        row_end = row_start + 1;
        column_start = tank->collision_rect.x / AppConfig::tile_rect.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_rect.w + 1;
        break;
    case D_LEFT:
        column_end = tank->collision_rect.x / AppConfig::tile_rect.w;
        column_start = column_end - 1;
        row_start = tank->collision_rect.y / AppConfig::tile_rect.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_rect.h + 1;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= m_level_columns_count) column_end = m_level_columns_count - 1;
    if(row_end >= m_level_rows_count) row_end = m_level_rows_count - 1;

    pr = tank->nextCollisionRect(dt);
    SDL_Rect intersect_rect;


    for(int i = row_start; i <= row_end; i++)
        for(int j = column_start; j <= column_end ;j++)
        {
            if(tank->stop) break;
            o = m_level.at(i).at(j);
            if(o == nullptr) continue;
            if(tank->testFlag(TSF_BOAT) && o->type == ST_WATER) continue;

            lr = &o->collision_rect;

            intersect_rect = intersectRect(lr, &pr);
            if(intersect_rect.w > 0 && intersect_rect.h > 0)
            {
                if(o->type == ST_ICE)
                {
                    if(intersect_rect.w > 10 && intersect_rect.h > 10)
                       tank->setFlag(TSF_ON_ICE);
                    continue;
                }
                else
                    tank->collide(intersect_rect);
                break;
            }
        }

    //========================kolizja z granicami mapy========================
    SDL_Rect outside_map_rect;
    //prostokąt po lewej stronie mapy
    outside_map_rect.x = -AppConfig::tile_rect.w;
    outside_map_rect.y = -AppConfig::tile_rect.h;
    outside_map_rect.w = AppConfig::tile_rect.w;
    outside_map_rect.h = AppConfig::map_rect.h + 2 * AppConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po prawej stronie mapy
    outside_map_rect.x = AppConfig::map_rect.w;
    outside_map_rect.y = -AppConfig::tile_rect.h;
    outside_map_rect.w = AppConfig::tile_rect.w;
    outside_map_rect.h = AppConfig::map_rect.h + 2 * AppConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po górnej stronie mapy
    outside_map_rect.x = 0;
    outside_map_rect.y = -AppConfig::tile_rect.h;
    outside_map_rect.w = AppConfig::map_rect.w;
    outside_map_rect.h = AppConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    //prostokąt po dolnej stronie mapy
    outside_map_rect.x = 0;
    outside_map_rect.y = AppConfig::map_rect.h;
    outside_map_rect.w = AppConfig::map_rect.w;
    outside_map_rect.h = AppConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);


   //========================kolizja z orzełkiem========================
    intersect_rect = intersectRect(&m_eagle->collision_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);
}

void NetworkBattle::checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt)
{
    SDL_Rect cr1 = tank1->nextCollisionRect(dt);
    SDL_Rect cr2 = tank2->nextCollisionRect(dt);
    SDL_Rect intersect_rect = intersectRect(&cr1, &cr2);

    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        //TODO emit event collision
        tank1->collide(intersect_rect);
        tank2->collide(intersect_rect);
    }
}

void NetworkBattle::checkCollisionBulletWithLevel(Bullet* bullet)
{
    if(bullet == nullptr) return;
    if(bullet->collide) return;

    int row_start, row_end;
    int column_start, column_end;

    SDL_Rect* br, *lr;
    SDL_Rect intersect_rect;
    Object* o;

    //========================kolizja z elementami mapy========================
    switch(bullet->direction)
    {
    case D_UP:
        row_start = row_end = bullet->collision_rect.y / AppConfig::tile_rect.h;
        column_start = bullet->collision_rect.x / AppConfig::tile_rect.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_rect.w;
        break;
    case D_RIGHT:
        column_start = column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_rect.w;
        row_start = bullet->collision_rect.y / AppConfig::tile_rect.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_rect.h;
        break;
    case D_DOWN:
        row_start = row_end = (bullet->collision_rect.y + bullet->collision_rect.h)/ AppConfig::tile_rect.h;
        column_start = bullet->collision_rect.x / AppConfig::tile_rect.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_rect.w;
        break;
    case D_LEFT:
        column_start = column_end = bullet->collision_rect.x / AppConfig::tile_rect.w;
        row_start = bullet->collision_rect.y / AppConfig::tile_rect.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_rect.h;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= m_level_columns_count) column_end = m_level_columns_count - 1;
    if(row_end >= m_level_rows_count) row_end = m_level_rows_count - 1;

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
                //TODO emit bullet collision
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
    if(br->x < 0 || br->y < 0 || br->x + br->w > AppConfig::map_rect.w || br->y + br->h > AppConfig::map_rect.h)
    {
        bullet->destroy();
    }
    //========================kolizja z orzełkiem========================
    if(m_eagle->type == ST_EAGLE && !m_game_over)
    {
        intersect_rect = intersectRect(&m_eagle->collision_rect, br);
        if(intersect_rect.w > 0 && intersect_rect.h > 0)
        {
            bullet->destroy();
            m_eagle->destroy();
            m_game_over_position = AppConfig::map_rect.h;
            m_game_over = true;
        }
    }
}

void NetworkBattle::checkCollisionBulletWithBush(Bullet *bullet)
{
    if(bullet == nullptr) return;
    if(bullet->collide) return;
    if(!bullet->increased_damage) return;

    SDL_Rect* br, *lr;
    SDL_Rect intersect_rect;
    br = &bullet->collision_rect;

    for(auto bush : m_bushes)
    {
        if(bush->to_erase) continue;
        lr = &bush->collision_rect;
        intersect_rect = intersectRect(lr, br);

        if(intersect_rect.w > 0 && intersect_rect.h > 0)
        {
            bullet->destroy();
            bush->to_erase = true;
        }
    }
}

void NetworkBattle::checkCollisionPlayerBulletsWithEnemy(Player *player, Enemy *enemy)
{
    if(player->to_erase || enemy->to_erase) return;
    if(enemy->testFlag(TSF_DESTROYED)) return;
    SDL_Rect intersect_rect;

    for(auto bullet : player->bullets)
    {
        if(!bullet->to_erase && !bullet->collide)
        {
            intersect_rect = intersectRect(&bullet->collision_rect, &enemy->collision_rect);
            if(intersect_rect.w > 0 && intersect_rect.h > 0)
            {
                if(enemy->testFlag(TSF_BONUS)) generateBonus();

                bullet->destroy();
                enemy->destroy();
                if(enemy->lives_count <= 0) m_enemy_to_kill--;
                player->score += enemy->scoreForHit();
            }
        }
    }
}

void NetworkBattle::checkCollisionEnemyBulletsWithPlayer(Enemy *enemy, Player *player)
{
    if(enemy->to_erase || player->to_erase) return;
    if(player->testFlag(TSF_DESTROYED)) return;
    SDL_Rect intersect_rect;

    for(auto bullet : enemy->bullets)
    {
        if(!bullet->to_erase && !bullet->collide)
        {
            intersect_rect = intersectRect(&bullet->collision_rect, &player->collision_rect);
            if(intersect_rect.w > 0 && intersect_rect.h > 0)
            {
                bullet->destroy();
                player->destroy();
            }
        }
    }
}

void NetworkBattle::checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2)
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

void NetworkBattle::checkCollisionPlayerWithBonus(Player *player, Bonus *bonus)
{
    if(player->to_erase || bonus->to_erase) return;

    SDL_Rect intersect_rect = intersectRect(&player->collision_rect, &bonus->collision_rect);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        player->score += 300;

        if(bonus->type == ST_BONUS_GRANATE)
        {
            for(auto enemy : m_enemies)
            {
                if(!enemy->to_erase)
                {
                    player->score += 200;
                    while(enemy->lives_count > 0) enemy->destroy();
                    m_enemy_to_kill--;
                }
            }
        }
        else if(bonus->type == ST_BONUS_HELMET)
        {
            player->setFlag(TSF_SHIELD);
        }
        else if(bonus->type == ST_BONUS_CLOCK)
        {
            for(auto enemy : m_enemies) if(!enemy->to_erase) enemy->setFlag(TSF_FROZEN);
        }
        else if(bonus->type == ST_BONUS_SHOVEL)
        {
            m_protect_eagle = true;
            m_protect_eagle_time = 0;
            for(int i = 0; i < 3; i++)
            {
                if(m_level.at(m_level_rows_count - i - 1).at(11) != nullptr)
                    delete m_level.at(m_level_rows_count - i - 1).at(11);
                m_level.at(m_level_rows_count - i - 1).at(11) = new Object(11 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1) * AppConfig::tile_rect.h, ST_STONE_WALL);

                if(m_level.at(m_level_rows_count - i - 1).at(14) != nullptr)
                    delete m_level.at(m_level_rows_count - i - 1).at(14);
                m_level.at(m_level_rows_count - i - 1).at(14) = new Object(14 * AppConfig::tile_rect.w, (m_level_rows_count - i - 1)  * AppConfig::tile_rect.h, ST_STONE_WALL);
            }
            for(int i = 12; i < 14; i++)
            {
                if(m_level.at(m_level_rows_count - 3).at(i) != nullptr)
                    delete m_level.at(m_level_rows_count - 3).at(i);
                m_level.at(m_level_rows_count - 3).at(i) = new Object(i * AppConfig::tile_rect.w, (m_level_rows_count - 3) * AppConfig::tile_rect.h, ST_STONE_WALL);
            }
        }
        else if(bonus->type == ST_BONUS_TANK)
        {
            player->lives_count++;
        }
        else if(bonus->type == ST_BONUS_STAR)
        {
            player->changeStarCountBy(1);
        }
        else if(bonus->type == ST_BONUS_GUN)
        {
            player->changeStarCountBy(3);
        }
        else if(bonus->type == ST_BONUS_BOAT)
        {
            player->setFlag(TSF_BOAT);
        }
        bonus->to_erase = true;
    }
}

void NetworkBattle::createSeeds()
{
//    srand(parent->getCurrentFrame());
    GenerateEvent* gen_rand = new GenerateEvent;
//    gen_rand->frame_number.l_value = parent->getCurrentFrame() + gen_rand->priority;
    gen_rand->seed1.l_value = time(0);
    gen_rand->seed2.l_value = time(0);
    gen_rand->seed3.l_value = time(0);

//    cout << "Seeds " << gen_rand->seed1.l_value << " " << gen_rand->seed2.l_value << " " << gen_rand->seed3.l_value << std::endl;

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->transmit_events.addEvent(gen_rand);
    LeaveCriticalSection(parent->critical_section);
}

void NetworkBattle::nextLevel()
{
    m_current_level++;
    if(m_current_level > 35) m_current_level = 1;
    if(m_current_level < 0) m_current_level = 35;

    m_level_start_screen = true;
    m_level_start_time = 0;
    m_game_over = false;
    m_finished = false;
    m_enemy_to_kill = AppConfig::enemy_start_count;

    std::string level_path = AppConfig::levels_path + Engine::intToString(m_current_level);
    loadLevel(level_path);

    if(m_players.empty())
    {
        if(m_player_count == 2)
        {
            Player* p1 = new Player(AppConfig::player_starting_point.at(0).x, AppConfig::player_starting_point.at(0).y, ST_PLAYER_1);
            Player* p2 = new Player(AppConfig::player_starting_point.at(1).x, AppConfig::player_starting_point.at(1).y, ST_PLAYER_2);
            p1->setParent(parent);
            p2->setParent(parent);
            NetworkState state;
            EnterCriticalSection(parent->critical_section);
                state = parent->shared_data->network_state;
            LeaveCriticalSection(parent->critical_section);
            if(state == NetworkState::SERVER)
            {
                p1->player_keys = AppConfig::player_keys.at(2);
            }
            else if(state == NetworkState::CLIENT || state == NetworkState::CLIENT_INITIALIZED)
            {
                p2->player_keys = AppConfig::player_keys.at(2);
            }

            vector<SOCKET> v;
            for(map<SOCKET,std::string>::iterator it = m_player_name.begin(); it != m_player_name.end(); ++it)
            {
              v.push_back(it->first);
            }

            p1->object_id = v[0];
            p2->object_id = v[1];

            m_players.push_back(p1);
            m_players.push_back(p2);

        }
        else
        {
            Player* p1 = new Player(AppConfig::player_starting_point.at(0).x, AppConfig::player_starting_point.at(0).y, ST_PLAYER_1);
            p1->setParent(parent);
            p1->player_keys = AppConfig::player_keys.at(2);

            vector<SOCKET> v;
            for(map<SOCKET,std::string>::iterator it = m_player_name.begin(); it != m_player_name.end(); ++it)
            {
              v.push_back(it->first);
            }

            p1->object_id = v[0];

            m_players.push_back(p1);
        }
    }
}

void NetworkBattle::generateEnemy()
{
    float p = static_cast<float>(rand()) / RAND_MAX;
    SpriteType type = static_cast<SpriteType>(p < (0.00735 * m_current_level + 0.09265) ? ST_TANK_D : rand() % (ST_TANK_C - ST_TANK_A + 1) + ST_TANK_A);
    Enemy* e = new Enemy(AppConfig::enemy_starting_point.at(m_enemy_respown_position).x, AppConfig::enemy_starting_point.at(m_enemy_respown_position).y, type);
    m_enemy_respown_position++;
    if(m_enemy_respown_position >= AppConfig::enemy_starting_point.size()) m_enemy_respown_position = 0;

    double a, b, c;
    if(m_current_level <= 17)
    {
        a = -0.040625 * m_current_level + 0.940625;
        b = -0.028125 * m_current_level + 0.978125;
        c = -0.014375 * m_current_level + 0.994375;
    }
    else
    {
        a = -0.012778 * m_current_level + 0.467222;
        b = -0.025000 * m_current_level + 0.925000;
        c = -0.036111 * m_current_level + 1.363889;
    }

    p = static_cast<float>(rand()) / RAND_MAX;
    if(p < a) e->lives_count = 1;
    else if(p < b) e->lives_count = 2;
    else if(p < c) e->lives_count = 3;
    else e->lives_count = 4;

    p = static_cast<float>(rand()) / RAND_MAX;
    if(p < 0.12) e->setFlag(TSF_BONUS);

    m_enemies.push_back(e);
}

void NetworkBattle::generateBonus()
{
    Bonus* b = new Bonus(0, 0, static_cast<SpriteType>(rand() % (ST_BONUS_BOAT - ST_BONUS_GRANATE + 1) + ST_BONUS_GRANATE));
    SDL_Rect intersect_rect;
    do
    {
        b->pos_x = rand() % (AppConfig::map_rect.x + AppConfig::map_rect.w - 1 *  AppConfig::tile_rect.w);
        b->pos_y = rand() % (AppConfig::map_rect.y + AppConfig::map_rect.h - 1 * AppConfig::tile_rect.h);
        b->update(0);
        intersect_rect = intersectRect(&b->collision_rect, &m_eagle->collision_rect);
    }while(intersect_rect.w > 0 && intersect_rect.h > 0);

    m_bonuses.push_back(b);
}