#include "scores.h"
#include "../engine/engine.h"
#include "../appconfig.h"
#include "game.h"
#include "menu.h"

Scores::Scores()
{
    m_show_time = 0;
    m_level = 0;
    m_game_over = true;
    m_score_counter_run = true;
    m_score_counter = 0;
    m_max_score = 0;
}

Scores::Scores(std::vector<Player *> players, int level, bool game_over)
{
    m_players = players;
    m_level = level;
    m_game_over = game_over;
    m_show_time = 0;
    m_score_counter_run = true;
    m_score_counter = 0;
    m_max_score = 0;
    for(auto player : m_players)
    {
        player->to_erase = false;
        if(player->lives_count == 0 && !game_over) player->lives_count = 2;
        else player->lives_count++;
        player->respawn();
        player->setFlag(TSF_MENU);

        if(player->score > m_max_score) m_max_score = player->score;
    }
}

void Scores::draw()
{
    Renderer* renderer = Engine::getEngine().getRenderer();
    renderer->clear();

    renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer->drawRect(&AppConfig::status_rect, {0, 0, 0, 255}, true);

    SDL_Point p_dst;
    SDL_Rect dst;

    p_dst = {-1, 10};
    renderer->drawText(&p_dst, std::string("STAGE ") + Engine::intToString(m_level), {255, 255, 220, 255}, 1);
    p_dst = {100, 50};
    renderer->drawText(&p_dst, std::string("PLAYER"), {255, 255, 255, 255}, 2);
    p_dst = {270, 50};
    renderer->drawText(&p_dst, std::string("SCORE"), {255, 255, 255, 255}, 2);
    dst = {75, 75, 300, 2};
    renderer->drawRect(&dst, {250, 250, 200, 255}, true);
    int i = 0;
    for(auto player : m_players)
    {
        dst = {100, 90 + i * (player->src_rect.h), player->src_rect.w, player->src_rect.h};
        renderer->drawObject(&player->src_rect, &dst);
        p_dst = {140, 98 + i * (player->src_rect.h)};
        renderer->drawText(&p_dst, std::string("x") + Engine::intToString(player->lives_count), {255, 255, 255, 255}, 2);
        p_dst = {270, 98 + i * (player->src_rect.h)};
        renderer->drawText(&p_dst, (m_score_counter < player->score ? Engine::intToString(m_score_counter) : Engine::intToString(player->score)), {255, 255, 255, 255}, 2);
        i++;
    }

    renderer->flush();
}

void Scores::update(Uint32 dt)
{
    if(m_score_counter > (1 << 30) || m_score_counter > m_max_score)
    {
        m_show_time += dt;
        m_score_counter_run = false;
    }
    if(m_score_counter_run)
    {
        if(m_score_counter < 20) m_score_counter++;
        else if(m_score_counter < 200) m_score_counter += 10;
        else if(m_score_counter < 2000) m_score_counter += 100;
        else if(m_score_counter < 20000) m_score_counter += 1000;
        else if(m_score_counter < 200000) m_score_counter += 10000;
        else m_score_counter += 100000;
    }
    for(auto player : m_players)
    {
        player->speed = player->default_speed;
        player->stop = true;
        player->setDirection(D_RIGHT);
        player->update(dt);
    }
}

void Scores::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_RETURN)
        {
            if(m_score_counter > (1 << 30)) m_show_time = AppConfig::score_show_time + 1;
            else m_score_counter = (1 << 30) + 1;
        }
    }
}

bool Scores::finished() const
{
    return m_show_time > AppConfig::score_show_time;
}

AppState *Scores::nextState()
{
    if(m_game_over)
    {
        Menu* m = new Menu;
        return m;
    }
    Game* g = new Game(m_players, m_level);
    return g;
}
