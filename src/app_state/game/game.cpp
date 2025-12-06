#include "game.h"
#include "../menu.h"
#include "../scores.h"
#include "../../engine/engine.h"
#include "../../engine/data/data.h"
#include "../../appconfig.h"
#include "../../spriteconfig.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>

Game::Game(int players_count)
{
    m_current_level = 1;
    m_player_count = players_count;
    m_pause = false;
    m_level_end_time = 0;
    m_enemy_respown_position = 0;
    m_level_environment = new LevelEnvironment(m_current_level);

    m_level_start_screen = true;
    m_level_start_time = 0;
    m_game_over = false;
    m_finished = false;
    m_enemy_to_kill = AppConfig::enemy_start_count;

    createPlayersIfNeeded();
}

Game::Game(std::vector<Player *> players, int previous_level)
{
    m_current_level = previous_level + 1;
    m_players = players;
    m_player_count = m_players.size();
    for (auto player : m_players)
    {
        player->resetKeyStates();
        player->clearFlag(Tank::TSF_MENU);
        player->lives_count++;
        player->respawn();
    }
    m_pause = false;
    m_level_end_time = 0;
    m_enemy_respown_position = 0;
    m_level_environment = new LevelEnvironment(m_current_level);

    m_level_start_screen = true;
    m_level_start_time = 0;
    m_game_over = false;
    m_finished = false;
    m_enemy_to_kill = AppConfig::enemy_start_count;

    createPlayersIfNeeded();
}

Game::~Game()
{
    clearAll();
}

void Game::draw(Renderer &renderer)
{
    renderer.clear();

    Point text_centered_pos = {-1, -1};
    if (m_level_start_screen)
    {
        drawLevelStartScreen(renderer);
    }
    else
    {
        renderer.drawRect(AppConfig::map_rect, {0, 0, 0, 0}, true);

        drawObjects(renderer);

        if (m_game_over)
        {
            drawGameOver(renderer);
        }

        drawGameStatusPanel(renderer);

        if (m_pause)
            renderer.drawText(text_centered_pos, std::string("PAUSE"), {200, 0, 0, 255}, 1);
    }

    renderer.flush();
}

void Game::update(Uint32 dt)
{
    if (dt > 40)
        return;

    if (m_level_start_screen)
    {
        if (m_level_start_time > AppConfig::level_start_time)
            m_level_start_screen = false;

        m_level_start_time += dt;
    }
    else
    {
        if (m_pause)
            return;

        checkCollisions(dt);
        updateObjects(dt);
        calculateEnemiesTargets();
        generateEnemyIfPossible(dt);

        if (m_enemies.empty() && m_enemy_to_kill <= 0)
        {
            m_level_end_time += dt;
            if (m_level_end_time > AppConfig::level_end_time)
                m_finished = true;
        }

        if (m_players.empty() && !m_game_over)
        {
            gameOver();
        }

        if (m_game_over)
        {
            if (m_game_over_message_position < 10)
                m_finished = true;
            else
                m_game_over_message_position -= AppConfig::game_over_message_speed * dt;
        }
    }
}

void Game::eventProcess(const Event &event)
{
    if (!m_level_start_screen && event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &event_key = static_cast<const KeyboardEvent &>(event);
        if (event_key.isPressed(KEY_N))
        {
            m_enemy_to_kill = 0;
            m_finished = true;
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_enemy_to_kill = 0;
            m_current_level -= 2;
            m_finished = true;
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_enemy_to_kill = 0;
            m_current_level -= 2;
            m_finished = true;
        }
        else if (event_key.isPressed(KEY_T))
        {
            AppConfig::show_enemy_target = !AppConfig::show_enemy_target;
        }
        else if (event_key.isPressed(KEY_RETURN))
        {
            m_pause = !m_pause;
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            m_finished = true;
        }

        if (!m_pause)
        {
            for (auto player : m_players)
                player->handleKeyboardEvent(event_key);
        }
    }
}

AppState *Game::nextState()
{
    if (!m_finished)
        return this;

    if (m_game_over || m_enemy_to_kill <= 0)
    {
        m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [this](Player *p)
                                       {m_killed_players.push_back(p); return true; }),
                        m_players.end());
        Scores *scores = new Scores(m_killed_players, m_current_level, m_game_over);
        return scores;
    }
    Menu *m = new Menu;
    return m;
}

void Game::clearAll()
{
    for (auto enemy : m_enemies)
        delete enemy;
    m_enemies.clear();

    for (auto player : m_players)
        delete player;
    m_players.clear();

    for (auto bonus : m_bonuses)
        delete bonus;
    m_bonuses.clear();

    delete m_level_environment;
    m_level_environment = nullptr;
}

void Game::createPlayersIfNeeded()
{
    if (m_players.empty())
    {
        if (m_player_count == 2)
        {
            Player *p1 = new Player(AppConfig::player_starting_point.at(0).x, AppConfig::player_starting_point.at(0).y, ST_PLAYER_1, AppConfig::player_1_keys);
            Player *p2 = new Player(AppConfig::player_starting_point.at(1).x, AppConfig::player_starting_point.at(1).y, ST_PLAYER_2, AppConfig::player_2_keys);
            m_players.push_back(p1);
            m_players.push_back(p2);
        }
        else
        {
            Player *p1 = new Player(AppConfig::player_starting_point.at(0).x, AppConfig::player_starting_point.at(0).y, ST_PLAYER_1, AppConfig::player_1_keys);
            m_players.push_back(p1);
        }
    }
}

void Game::drawLevelStartScreen(Renderer &renderer)
{

    Point text_centered_pos = {-1, -1};
    std::string level_name = "STAGE " + std::to_string(m_current_level);
    renderer.drawText(text_centered_pos, level_name, {255, 255, 255, 255}, 1);
}
void Game::drawObjects(Renderer &renderer)
{
    m_level_environment->drawFirstLayer(renderer);

    for (auto player : m_players)
        player->draw(renderer);
    for (auto enemy : m_enemies)
        enemy->draw(renderer);

    for (auto bonus : m_bonuses)
        bonus->draw(renderer);

    m_level_environment->drawSecondLayer(renderer);
}

void Game::drawGameOver(Renderer &renderer)
{
    Point pos = {-1, m_game_over_message_position};
    renderer.drawText(pos, AppConfig::game_over_text, {255, 10, 10, 255});
}

void Game::drawGameStatusPanel(Renderer &renderer)
{
    // enemies left to kill
    Rect enemy_left_src = SpriteConfig::getInstance().getSpriteData(ST_LEFT_ENEMY).rect;
    for (int i = 0; i < m_enemy_to_kill; i++)
    {
        Rect enemy_status_dst = {
            AppConfig::status_rect.x + 8 + enemy_left_src.w * (i % 2),
            5 + enemy_left_src.h * (i / 2),
            enemy_left_src.w,
            enemy_left_src.h};
        renderer.drawObject(enemy_left_src, enemy_status_dst);
    }
    // players' lives
    int player_y_offset = (AppConfig::enemy_start_count / 2) * enemy_left_src.h + 20;
    int i = 0;
    for (auto player : m_players)
    {
        Rect player_status_dst = {AppConfig::status_rect.x + 5, player_y_offset + i * (AppConfig::tile_size.h + 2), AppConfig::tile_size.w, AppConfig::tile_size.h};
        Point player_lives_dst = {player_status_dst.x + player_status_dst.w + 2, player_status_dst.y + 3};
        i++;
        renderer.drawObject(player->src_rect, player_status_dst);
        renderer.drawText(player_lives_dst, std::to_string(player->lives_count), {0, 0, 0, 255}, 3);
    }
    // current level
    Rect stage_status_src = SpriteConfig::getInstance().getSpriteData(ST_STAGE_STATUS).rect;

    int stage_status_y_offset = player_y_offset + i * ((m_players.size() + m_killed_players.size()) * (AppConfig::tile_size.h + 2) + 10);
    Rect stage_status_dst = {AppConfig::status_rect.x + 8, stage_status_y_offset, stage_status_src.w, stage_status_src.h};
    Point stage_number_dst = {stage_status_dst.x + 10, stage_status_dst.y + 26};

    renderer.drawObject(stage_status_src, stage_status_dst);
    renderer.drawText(stage_number_dst, std::to_string(m_current_level), {0, 0, 0, 255}, 2);
}

void Game::checkCollisions(Uint32 dt)
{
    std::vector<Player *>::iterator pl1, pl2;
    std::vector<Enemy *>::iterator en1, en2;

    for (pl1 = m_players.begin(); pl1 != m_players.end(); pl1++)
        for (pl2 = pl1 + 1; pl2 != m_players.end(); pl2++)
            checkCollisionTwoTanks(*pl1, *pl2, dt);

    for (en1 = m_enemies.begin(); en1 != m_enemies.end(); en1++)
        for (en2 = en1 + 1; en2 != m_enemies.end(); en2++)
            checkCollisionTwoTanks(*en1, *en2, dt);

    for (auto enemy : m_enemies)
    {
        m_level_environment->checkCollisionTankWithLevel(enemy, dt);
        for (auto bullet : enemy->bullets)
        {
            m_level_environment->checkCollisionBulletWithLevel(bullet);
            if (m_level_environment->checkCollisionBulletWithEagle(bullet))
            {
                gameOver();
            }
        }
    }
    for (auto player : m_players)
    {
        m_level_environment->checkCollisionTankWithLevel(player, dt);
        for (auto bullet : player->bullets)
        {
            m_level_environment->checkCollisionBulletWithLevel(bullet);
            if (m_level_environment->checkCollisionBulletWithEagle(bullet))
            {
                gameOver();
            }
        }
    }

    for (auto player : m_players)
        for (auto enemy : m_enemies)
        {
            checkCollisionTwoTanks(player, enemy, dt);

            checkCollisionPlayerBulletsWithEnemy(player, enemy);
            checkCollisionEnemyBulletsWithPlayer(enemy, player);

            for (auto bullet1 : player->bullets)
                for (auto bullet2 : enemy->bullets)
                    checkCollisionTwoBullets(bullet1, bullet2);
        }

    for (auto player : m_players)
        for (auto bonus : m_bonuses)
            checkCollisionPlayerWithBonus(player, bonus);
}

void Game::checkCollisionTwoTanks(Tank *tank1, Tank *tank2, Uint32 dt)
{
    Rect cr1 = tank1->nextCollisionRect(dt);
    Rect cr2 = tank2->nextCollisionRect(dt);
    Rect intersect_rect = intersectRect(&cr1, &cr2);

    if (intersect_rect.isNotEmpty())
    {
        tank1->collide(intersect_rect);
        tank2->collide(intersect_rect);
    }
}

void Game::checkCollisionPlayerBulletsWithEnemy(Player *player, Enemy *enemy)
{
    if (player->to_erase || enemy->to_erase)
        return;
    if (enemy->testFlag(Tank::TSF_DESTROYED))
        return;
    Rect intersect_rect;

    for (auto bullet : player->bullets)
    {
        if (!bullet->to_erase && !bullet->collide)
        {
            intersect_rect = intersectRect(&bullet->collision_rect, &enemy->collision_rect);
            if (intersect_rect.isNotEmpty())
            {
                if (enemy->testFlag(Tank::TSF_BONUS))
                    generateBonus();

                bullet->destroy();
                enemy->destroy();
                if (enemy->lives_count <= 0)
                    m_enemy_to_kill--;
                player->score += enemy->scoreForHit();
            }
        }
    }
}

void Game::checkCollisionEnemyBulletsWithPlayer(Enemy *enemy, Player *player)
{
    if (enemy->to_erase || player->to_erase)
        return;
    if (player->testFlag(Tank::TSF_DESTROYED))
        return;
    Rect intersect_rect;

    for (auto bullet : enemy->bullets)
    {
        if (!bullet->to_erase && !bullet->collide)
        {
            intersect_rect = intersectRect(&bullet->collision_rect, &player->collision_rect);
            if (intersect_rect.isNotEmpty())
            {
                bullet->destroy();
                player->destroy();
            }
        }
    }
}

void Game::checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2)
{
    if (bullet1 == nullptr || bullet2 == nullptr)
        return;
    if (bullet1->to_erase || bullet2->to_erase)
        return;

    Rect intersect_rect = intersectRect(&bullet1->collision_rect, &bullet2->collision_rect);

    if (intersect_rect.isNotEmpty())
    {
        bullet1->destroy();
        bullet2->destroy();
    }
}

void Game::checkCollisionPlayerWithBonus(Player *player, Bonus *bonus)
{
    if (player->to_erase || bonus->to_erase)
        return;

    Rect intersect_rect = intersectRect(&player->collision_rect, &bonus->collision_rect);
    if (intersect_rect.isNotEmpty())
    {
        player->score += 300;

        if (bonus->type == ST_BONUS_GRENADE)
        {
            for (auto enemy : m_enemies)
            {
                if (!enemy->to_erase)
                {
                    player->score += 200;
                    while (enemy->lives_count > 0)
                        enemy->destroy();
                    m_enemy_to_kill--;
                }
            }
        }
        else if (bonus->type == ST_BONUS_HELMET)
        {
            player->setFlag(Tank::TSF_SHIELD);
        }
        else if (bonus->type == ST_BONUS_CLOCK)
        {
            for (auto enemy : m_enemies)
                if (!enemy->to_erase)
                    enemy->setFlag(Tank::TSF_FROZEN);
        }
        else if (bonus->type == ST_BONUS_SHOVEL)
        {
            m_level_environment->startEagleProtection();
        }
        else if (bonus->type == ST_BONUS_TANK)
        {
            player->lives_count++;
        }
        else if (bonus->type == ST_BONUS_STAR)
        {
            player->changeStarCountBy(1);
        }
        else if (bonus->type == ST_BONUS_GUN)
        {
            player->changeStarCountBy(3);
        }
        else if (bonus->type == ST_BONUS_BOAT)
        {
            player->setFlag(Tank::TSF_BOAT);
        }
        bonus->to_erase = true;
    }
}

void Game::updateObjects(Uint32 dt)
{
    for (auto enemy : m_enemies)
        enemy->update(dt);
    for (auto player : m_players)
        player->update(dt);
    for (auto bonus : m_bonuses)
        bonus->update(dt);

    m_level_environment->update(dt);

    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](Enemy *e)
                                   {if(e->to_erase) {delete e; return true;} return false; }),
                    m_enemies.end());
    m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [this](Player *p)
                                   {if(p->to_erase) {m_killed_players.push_back(p); return true;} return false; }),
                    m_players.end());
    m_bonuses.erase(std::remove_if(m_bonuses.begin(), m_bonuses.end(), [](Bonus *b)
                                   {if(b->to_erase) {delete b; return true;} return false; }),
                    m_bonuses.end());
}

void Game::gameOver()
{
    m_level_environment->destroyEagle();
    m_game_over_message_position = AppConfig::map_rect.h;
    m_game_over = true;
}

void Game::calculateEnemiesTargets()
{
    int min_metric;
    Point target;
    for (auto enemy : m_enemies)
    {
        Point enemy_center = enemy->getCenter();
        min_metric = AppConfig::map_rect.w + AppConfig::map_rect.h;
        if (enemy->type == ST_TANK_A || enemy->type == ST_TANK_D)
        {
            for (auto player : m_players)
            {
                Point player_center = player->getCenter();
                int metric = fabs(player_center.x - enemy_center.x) + fabs(player_center.y - enemy_center.y);
                if (metric < min_metric)
                {
                    min_metric = metric;
                    target = player_center;
                }
            }
        }

        Point eagle_center = m_level_environment->getEagleCenter();
        int metric = fabs(eagle_center.x - enemy_center.x) + fabs(eagle_center.y - enemy_center.y);
        if (metric < min_metric)
        {
            min_metric = metric;
            target = eagle_center;
        }

        enemy->target_position = target;
    }
}

void Game::generateEnemyIfPossible(Uint32 dt)
{
    m_new_enemy_cooldown += dt;
    long unsigned allowed_enemies_count = AppConfig::enemy_max_count_on_map < m_enemy_to_kill ? AppConfig::enemy_max_count_on_map : m_enemy_to_kill;
    if (m_enemies.size() >= allowed_enemies_count || m_new_enemy_cooldown < AppConfig::new_enemy_cooldown)
        return;

    m_new_enemy_cooldown = 0;

    float p = static_cast<float>(rand()) / RAND_MAX;
    // magic numbers to calculate enemy type probabilities
    SpriteType type = static_cast<SpriteType>(p < (0.00735 * m_current_level + 0.09265) ? ST_TANK_D : (rand() % (ST_TANK_C - ST_TANK_A + 1) + ST_TANK_A));
    Enemy *e = new Enemy(AppConfig::enemy_starting_point.at(m_enemy_respown_position).x, AppConfig::enemy_starting_point.at(m_enemy_respown_position).y, type);
    m_enemy_respown_position++;
    if (m_enemy_respown_position >= AppConfig::enemy_starting_point.size())
        m_enemy_respown_position = 0;

    // magic numbers to calculate enemy armor probabilities
    double a, b, c;
    if (m_current_level <= 17)
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
    if (p < a)
        e->lives_count = 1;
    else if (p < b)
        e->lives_count = 2;
    else if (p < c)
        e->lives_count = 3;
    else
        e->lives_count = 4;

    p = static_cast<float>(rand()) / RAND_MAX;
    if (p < 0.12)
        e->setFlag(Tank::TSF_BONUS);

    m_enemies.push_back(e);
}

void Game::generateBonus()
{
    Bonus *b = new Bonus(0, 0, static_cast<SpriteType>(rand() % (ST_BONUS_BOAT - ST_BONUS_GRENADE + 1) + ST_BONUS_GRENADE));
    do
    {
        b->pos_x = rand() % (AppConfig::map_rect.x + AppConfig::map_rect.w - 1 * AppConfig::tile_size.w);
        b->pos_y = rand() % (AppConfig::map_rect.y + AppConfig::map_rect.h - 1 * AppConfig::tile_size.h);
        b->update(0);
    } while (m_level_environment->checkCollisionWithEagle(b->collision_rect));

    m_bonuses.push_back(b);
}
