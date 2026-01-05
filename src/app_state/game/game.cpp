#include "game.h"
#include "../menu.h"
#include "../scores.h"
#include "../../engine/engine.h"
#include "../../engine/data/data.h"
#include "../../appconfig.h"
#include "../../spriteconfig.h"
#include "../../soundconfig.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>

Game::Game(int players_count, InteractiveComponents interactive_components, StateMachine *parent_state_machine)
    : AppState(interactive_components, parent_state_machine)
{
    m_current_level = 1;
    m_players_count = players_count;
    m_pause = false;
    m_level_end_time = 0;
    m_enemy_respown_position = 0;
    m_level_environment = new LevelEnvironment(m_current_level, interactive_components);

    m_level_start_screen = true;
    m_level_start_time = 0;
    m_game_over = false;
    m_enemies_to_kill_count = AppConfig::enemies_to_kill_total_count;
    m_show_enemies_targets = false;

    createPlayersIfNeeded();
    playSound(SoundConfig::STAGE_START_UP);
}

Game::Game(std::vector<Player *> players, int previous_level, InteractiveComponents interactive_components, StateMachine *parent_state_machine)
    : AppState(interactive_components, parent_state_machine)
{
    m_current_level = previous_level + 1;
    m_players = players;
    m_players_count = m_players.size();
    for (auto player : m_players)
    {
        player->moveToNextLevel();
    }
    m_pause = false;
    m_level_end_time = 0;
    m_enemy_respown_position = 0;
    m_level_environment = new LevelEnvironment(m_current_level, interactive_components);

    m_level_start_screen = true;
    m_level_start_time = 0;
    m_game_over = false;
    m_enemies_to_kill_count = AppConfig::enemies_to_kill_total_count;
    m_show_enemies_targets = false;

    createPlayersIfNeeded();
    playSound(SoundConfig::STAGE_START_UP);
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
            renderer.drawText(text_centered_pos, std::string("PAUSE"), {200, 0, 0, 255}, FontSize::BIGGEST);
    }

    renderer.flush();
}

void Game::update(const UpdateState &updateState)
{
    Uint32 dt = updateState.delta_time;

    if (dt > 40)
        return;

    if (m_level_start_screen)
    {
        if (m_level_start_time > AppConfig::level_start_time)
        {
            m_level_start_screen = false;
        }

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

        if (m_enemies.empty() && m_enemies_to_kill_count <= 0)
        {
            m_level_end_time += dt;
            if (m_level_end_time > AppConfig::level_end_time)
            {
                transiteToNextState();
                return;
            }
        }

        if (m_players.empty() && !m_game_over)
        {
            gameOver();
        }

        if (m_game_over)
        {
            if (m_game_over_message_position < 10)
            {
                transiteToNextState();
                return;
            }
            else
            {
                m_game_over_message_position -= AppConfig::game_over_message_speed * dt;
            }
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
            m_enemies_to_kill_count = 0;
            transiteToNextState();
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_enemies_to_kill_count = 0;
            m_current_level -= 2;
            transiteToNextState();
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_enemies_to_kill_count = 0;
            m_current_level -= 2;
            transiteToNextState();
        }
        else if (event_key.isPressed(KEY_T))
        {
            m_show_enemies_targets = !m_show_enemies_targets;
        }
        else if (event_key.isPressed(KEY_RETURN) && !m_game_over)
        {
            m_pause = !m_pause;
            if (m_pause)
            {
                stopAllSounds();
                playSound(SoundConfig::PAUSE);
            }
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            transiteToNextState();
        }

        if (!m_pause && !m_game_over)
        {
            for (auto player : m_players)
                player->handleKeyboardEvent(event_key);
        }
    }
}

void Game::transiteToNextState()
{
    if (m_game_over || m_enemies_to_kill_count <= 0)
    {
        m_players.erase(std::remove_if(m_players.begin(), m_players.end(), [this](Player *p)
                                       {m_killed_players.push_back(p); return true; }),
                        m_players.end());
        transiteTo(new Scores(m_killed_players, m_current_level, m_game_over, m_interactive_components, m_state_machine));
    }
    else
    {
        transiteTo(new Menu(m_interactive_components, m_state_machine));
    }
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
        auto p1_starting_point = AppConfig::player_starting_point.at(0);
        auto p2_starting_point = AppConfig::player_starting_point.at(1);
        if (m_players_count == 2)
        {
            Player *p1 = new Player(p1_starting_point.x, p1_starting_point.y, ST_PLAYER_1, AppConfig::player_1_keys, m_interactive_components);
            Player *p2 = new Player(p2_starting_point.x, p2_starting_point.y, ST_PLAYER_2, AppConfig::player_2_keys, m_interactive_components);
            m_players.push_back(p1);
            m_players.push_back(p2);
        }
        else
        {
            Player *p1 = new Player(p1_starting_point.x, p1_starting_point.y, ST_PLAYER_1, AppConfig::player_1_keys, m_interactive_components);
            m_players.push_back(p1);
        }
    }
}

void Game::drawLevelStartScreen(Renderer &renderer)
{

    Point text_centered_pos = {-1, -1};
    std::string level_name = "STAGE " + std::to_string(m_current_level);
    renderer.drawText(text_centered_pos, level_name, {255, 255, 255, 255}, FontSize::BIGGEST);
}
void Game::drawObjects(Renderer &renderer)
{
    m_level_environment->drawFirstLayer(renderer);

    for (auto player : m_players)
        player->draw(renderer);
    for (auto enemy : m_enemies)
        drawEnemy(renderer, enemy);

    for (auto bonus : m_bonuses)
        bonus->draw(renderer);

    m_level_environment->drawSecondLayer(renderer);
}

void Game::drawEnemy(Renderer &renderer, Enemy *enemy)
{
    if (m_show_enemies_targets)
    {
        SpriteType type = enemy->type;
        Rect dest_rect = enemy->dest_rect;
        Point target_position = enemy->target_position;
        Color c;
        if (type == ST_TANK_A)
            c = {250, 0, 0, 250};
        if (type == ST_TANK_B)
            c = {0, 0, 250, 255};
        if (type == ST_TANK_C)
            c = {0, 255, 0, 250};
        if (type == ST_TANK_D)
            c = {250, 0, 255, 250};
        Rect r = {min(target_position.x, dest_rect.x + dest_rect.w / 2), dest_rect.y + dest_rect.h / 2, abs(target_position.x - (dest_rect.x + dest_rect.w / 2)), 1};
        renderer.drawRect(r, c, true);
        r = {target_position.x, min(target_position.y, dest_rect.y + dest_rect.h / 2), 1, abs(target_position.y - (dest_rect.y + dest_rect.h / 2))};
        renderer.drawRect(r, c, true);
    }

    enemy->draw(renderer);
}

void Game::drawGameOver(Renderer &renderer)
{
    Point pos = {-1, m_game_over_message_position};
    renderer.drawText(pos, AppConfig::game_over_text, {255, 10, 10, 255}, FontSize::BIGGEST);
}

void Game::drawGameStatusPanel(Renderer &renderer)
{
    // enemies left to kill
    Rect enemy_left_src = SpriteConfig::getInstance().getSpriteData(ST_LEFT_ENEMY).rect;
    for (int i = 0; i < m_enemies_to_kill_count; i++)
    {
        Rect enemy_status_dst = {
            AppConfig::status_rect.x + 8 + enemy_left_src.w * (i % 2),
            5 + enemy_left_src.h * (i / 2),
            enemy_left_src.w,
            enemy_left_src.h};
        renderer.drawObject(enemy_left_src, enemy_status_dst);
    }
    // players' lives
    int player_y_offset = (AppConfig::enemies_to_kill_total_count / 2) * enemy_left_src.h + 20;
    int i = 0;
    for (auto player : m_players)
    {
        Rect player_status_dst = {AppConfig::status_rect.x + 5, player_y_offset + i * (AppConfig::tile_size.h + 2), AppConfig::tile_size.w, AppConfig::tile_size.h};
        Point player_lives_dst = {player_status_dst.x + player_status_dst.w + 2, player_status_dst.y + 3};
        i++;
        renderer.drawObject(player->src_rect, player_status_dst);
        renderer.drawText(player_lives_dst, std::to_string(player->lives()), {0, 0, 0, 255}, FontSize::NORMAL);
    }
    // current level
    Rect stage_status_src = SpriteConfig::getInstance().getSpriteData(ST_STAGE_STATUS).rect;

    int stage_status_y_offset = player_y_offset + i * ((m_players.size() + m_killed_players.size()) * (AppConfig::tile_size.h + 2) + 10);
    Rect stage_status_dst = {AppConfig::status_rect.x + 8, stage_status_y_offset, stage_status_src.w, stage_status_src.h};
    Point stage_number_dst = {stage_status_dst.x + 10, stage_status_dst.y + 26};

    renderer.drawObject(stage_status_src, stage_status_dst);
    renderer.drawText(stage_number_dst, std::to_string(m_current_level), {0, 0, 0, 255}, FontSize::NORMAL);
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
            if (!m_game_over && m_level_environment->checkCollisionBulletWithEagle(bullet))
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
            if (!m_game_over && m_level_environment->checkCollisionBulletWithEagle(bullet))
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
    Rect intersect_rect = cr1.intersection(cr2);

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
        if (!bullet->to_erase && !bullet->isColide())
        {
            intersect_rect = bullet->collision_rect.intersection(enemy->collision_rect);
            if (intersect_rect.isNotEmpty())
            {
                if (enemy->testFlag(Tank::TSF_BONUS))
                    generateBonus();

                bullet->destroy();
                enemy->hit();
                player->addScore(enemy->scoreForHit());

                if (!enemy->alive())
                    m_enemies_to_kill_count--;
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
        if (!bullet->to_erase && !bullet->isColide())
        {
            intersect_rect = bullet->collision_rect.intersection(player->collision_rect);
            if (intersect_rect.isNotEmpty())
            {
                bullet->destroy();
                player->hit();
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

    Rect intersect_rect = bullet1->collision_rect.intersection(bullet2->collision_rect);

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

    Rect intersect_rect = player->collision_rect.intersection(bonus->collision_rect);
    if (intersect_rect.isNotEmpty())
    {
        player->addScore(300);

        if (bonus->type == ST_BONUS_GRENADE)
        {
            for (auto enemy : m_enemies)
            {
                if (!enemy->to_erase)
                {
                    player->addScore(200);
                    while (enemy->alive())
                    {
                        enemy->hit();
                    }
                    m_enemies_to_kill_count--;
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
            player->addLife();
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

        if (bonus->type == ST_BONUS_TANK)
        {
            playSound(SoundConfig::PLAYER_LIFE_UP);
        }
        else
        {
            playSound(SoundConfig::BONUS_OBTAINED);
        }
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

    bool player_tried_to_move = false;
    for (auto player : m_players)
        if (player->speed() > 0.0)
            player_tried_to_move = true;

    if (player_tried_to_move)
    {
        stopSound(SoundConfig::PLAYER_IDLE);
        playSound(SoundConfig::PLAYER_MOVING);
    }
    else
    {
        stopSound(SoundConfig::PLAYER_MOVING);
        playSound(SoundConfig::PLAYER_IDLE);
    }

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
    playSound(SoundConfig::GAME_OVER);
}

void Game::calculateEnemiesTargets()
{
    int min_metric;
    Point target;
    for (auto enemy : m_enemies)
    {
        Point enemy_center = enemy->center();
        min_metric = AppConfig::map_rect.w + AppConfig::map_rect.h;
        if (enemy->type == ST_TANK_A || enemy->type == ST_TANK_D)
        {
            for (auto player : m_players)
            {
                Point player_center = player->center();
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
    long unsigned allowed_enemies_count = AppConfig::enemies_max_count_on_map < m_enemies_to_kill_count ? AppConfig::enemies_max_count_on_map : m_enemies_to_kill_count;
    if (m_enemies.size() >= allowed_enemies_count || m_new_enemy_cooldown < AppConfig::new_enemy_cooldown)
        return;

    m_new_enemy_cooldown = 0;

    float p = static_cast<float>(rand()) / RAND_MAX;
    // magic numbers to calculate enemy type probabilities
    SpriteType type = static_cast<SpriteType>(p < (0.00735 * m_current_level + 0.09265) ? ST_TANK_D : (rand() % (ST_TANK_C - ST_TANK_A + 1) + ST_TANK_A));

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

    unsigned armor_count;

    p = static_cast<float>(rand()) / RAND_MAX;
    if (p < a)
        armor_count = 1;
    else if (p < b)
        armor_count = 2;
    else if (p < c)
        armor_count = 3;
    else
        armor_count = 4;

    auto starting_points = AppConfig::enemy_starting_point;
    auto starting_point = starting_points.at(m_enemy_respown_position);
    Enemy *e = new Enemy(starting_point.x, starting_point.y, type, armor_count, m_interactive_components);
    m_enemy_respown_position++;
    if (m_enemy_respown_position >= starting_points.size())
        m_enemy_respown_position = 0;

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

    playSound(SoundConfig::BONUS_APPEARED);

    m_bonuses.push_back(b);
}
