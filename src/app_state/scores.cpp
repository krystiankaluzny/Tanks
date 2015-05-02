#include "scores.h"
#include "../engine/engine.h"
#include "../appconfig.h"
#include "game.h"

Scores::Scores()
{
    m_show_time = 0;
}

Scores::Scores(std::vector<Player *> players, int level)
{
    m_players = players;
    m_show_time = 0;
    m_level = level;

    for(auto player : m_players)
    {
        player->setDirection(D_RIGHT);
        player->setFlag(TSF_LIFE);
        player->update(0);
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
    renderer->drawText(&p_dst, std::string("STAGE ") + AppState::intToString(m_level), {255, 255, 220, 255}, 1);
    p_dst = {100, 45};
    renderer->drawText(&p_dst, std::string("PLAYER"), {255, 255, 255, 255}, 2);
    p_dst = {270, 45};
    renderer->drawText(&p_dst, std::string("SCORE"), {255, 255, 255, 255}, 2);
    dst = {75, 70, 300, 2};
    renderer->drawRect(&dst, {250, 250, 200, 255}, true);
    int i = 0;
    for(auto player : m_players)
    {
        dst = {100, 70 + i * (player->src_rect.h), player->src_rect.w, player->src_rect.h};
        renderer->drawObject(&player->src_rect, &dst);

        p_dst = {270, 80 + i * (player->src_rect.h)};
        renderer->drawText(&p_dst, AppState::intToString(player->score), {255, 255, 255, 255}, 2);
        i++;
    }

    renderer->flush();
}

void Scores::update(Uint32 dt)
{
    m_show_time += dt;
}

void Scores::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_RETURN)
        {
            m_show_time = AppConfig::score_show_time;
        }
    }
}

bool Scores::finished() const
{
    return m_show_time > AppConfig::score_show_time;
}

AppState *Scores::nextState()
{
    Game* g = new Game(m_players, m_level);
    return g;
}
