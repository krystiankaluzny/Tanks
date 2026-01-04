#include "scores.h"
#include "../engine/engine.h"
#include "../engine/data/data.h"
#include "../appconfig.h"
#include "../soundconfig.h"
#include "game/game.h"
#include "menu.h"

Scores::Scores(std::vector<Player *> players, int level, bool game_over, InteractiveComponents interactive_components)
    : AppState(interactive_components)
{
    m_players = players;
    m_level = level;
    m_game_over = game_over;
    m_show_time = 0;
    m_score_count_time = 0;
    m_score_counter_run = true;
    m_score_counter = 0;
    m_max_score = 0;
    for (auto player : m_players)
    {
        player->to_erase = false;

        player->creatingState();
        player->setFlag(Tank::TSF_PREVIEW);

        if (player->score() > m_max_score)
            m_max_score = player->score();
    }

    stopAllSounds();
}

void Scores::draw(Renderer &renderer)
{
    renderer.clear();

    renderer.drawRect(AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer.drawRect(AppConfig::status_rect, {0, 0, 0, 255}, true);

    Point p_dst;
    Rect dst;

    p_dst = {-1, 10};
    renderer.drawText(p_dst, std::string("STAGE ") + std::to_string(m_level), {255, 255, 220, 255}, FontSize::BIGGEST);
    p_dst = {100, 50};
    renderer.drawText(p_dst, std::string("PLAYER"), {255, 255, 255, 255}, FontSize::NORMAL);
    p_dst = {270, 50};
    renderer.drawText(p_dst, std::string("SCORE"), {255, 255, 255, 255}, FontSize::NORMAL);
    dst = {75, 75, 300, 2};
    renderer.drawRect(dst, {250, 250, 200, 255}, true);
    int i = 0;
    for (auto player : m_players)
    {
        dst = {100, 90 + i * (player->src_rect.h), player->src_rect.w, player->src_rect.h};
        renderer.drawObject(player->src_rect, dst);
        p_dst = {140, 98 + i * (player->src_rect.h)};
        renderer.drawText(p_dst, std::string("x") + std::to_string(player->lives()), {255, 255, 255, 255}, FontSize::NORMAL);
        p_dst = {270, 98 + i * (player->src_rect.h)};
        renderer.drawText(p_dst, (m_score_counter < player->score() ? std::to_string(m_score_counter) : std::to_string(player->score())), {255, 255, 255, 255}, FontSize::NORMAL);
        i++;
    }

    renderer.flush();
}

void Scores::update(const UpdateState &updateState)
{
    Uint32 dt = updateState.delta_time;
    m_show_time += dt;
    m_score_count_time += dt;

    if (m_score_counter > (1 << 30) || m_score_counter >= m_max_score)
    {
        m_score_counter_run = false;
    }

    if (m_show_time > AppConfig::score_show_time)
        m_finished = true;

    if (m_score_counter_run)
    {
        if (m_score_count_time > AppConfig::score_count_time)
        {
            if (m_score_counter < 10)
                m_score_counter++;
            else if (m_score_counter < 100)
                m_score_counter += 10;
            else if (m_score_counter < 1000)
                m_score_counter += 100;
            else if (m_score_counter < 10000)
                m_score_counter += 1000;
            else if (m_score_counter < 100000)
                m_score_counter += 10000;
            else
                m_score_counter += 100000;

            playSound(SoundConfig::SCORE_POINT_COUNTED);

            m_score_count_time = 0;
        }
    }

    for (auto player : m_players)
    {
        player->setDirection(D_RIGHT);
        player->update(dt);
    }
}

void Scores::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

        if (ev.isPressed(KeyCode::KEY_RETURN))
        {
            if (m_score_counter > (1 << 30) || m_score_counter_run == false)
                m_finished = true;
            else
                m_score_counter = (1 << 30) + 1;
        }
    }
}

AppState *Scores::nextState()
{
    if (!m_finished)
        return this;

    if (m_game_over)
    {
        Menu *m = new Menu(m_interactive_components);
        return m;
    }
    Game *g = new Game(m_players, m_level, m_interactive_components);
    return g;
}
