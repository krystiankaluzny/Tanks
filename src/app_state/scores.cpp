#include "scores.h"
#include "../engine/engine.h"
#include "../engine/data/data.h"
#include "../appconfig.h"
#include "../soundconfig.h"
#include "game/game.h"
#include "menu.h"

Scores::Scores(std::vector<Player *> players, int level, bool game_over, InteractiveComponents interactive_components, StateMachine *parent_state_machine)
    : AppState(interactive_components, parent_state_machine)
{
    m_players = players;
    m_level = level;
    m_game_over = game_over;

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

    setSubState(new CountingState(this));
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

    for (auto player : m_players)
    {
        player->setDirection(D_RIGHT);
        player->update(dt);
    }
}

void Scores::eventProcess(const Event &event)
{
}

void Scores::transiteToNextState()
{
    if (m_game_over)
    {
        Menu *m = new Menu(m_interactive_components, m_state_machine);
        transiteTo(m);
    }
    else
    {
        Game *g = new Game(m_players, m_level, m_interactive_components, m_state_machine);
        transiteTo(g);
    }
}

Scores::CountingState::CountingState(Scores *ps) : SubState<Scores>(ps), m_single_score_count_time(0) {}

void Scores::CountingState::update(const UpdateState &updateState)
{
    Uint32 dt = updateState.delta_time;

    m_single_score_count_time += dt;

    if (m_single_score_count_time > AppConfig::Score::single_count_time)
    {
        if (m_parent_state->m_score_counter < 10)
            m_parent_state->m_score_counter++;
        else if (m_parent_state->m_score_counter < 100)
            m_parent_state->m_score_counter += 10;
        else if (m_parent_state->m_score_counter < 1000)
            m_parent_state->m_score_counter += 100;
        else if (m_parent_state->m_score_counter < 10000)
            m_parent_state->m_score_counter += 1000;
        else if (m_parent_state->m_score_counter < 100000)
            m_parent_state->m_score_counter += 10000;
        else
            m_parent_state->m_score_counter += 100000;

        m_parent_state->playSound(SoundConfig::SCORE_POINT_COUNTED);

        m_single_score_count_time = 0;
    }

    if (m_parent_state->m_score_counter >= m_parent_state->m_max_score)
    {
        transiteTo(new Scores::IdleState(m_parent_state));
    }
}

void Scores::CountingState::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

        if (ev.isPressed(KeyCode::KEY_RETURN))
        {
            m_parent_state->m_score_counter = m_parent_state->m_max_score;
            transiteTo(new Scores::IdleState(m_parent_state));
        }
    }
}

Scores::IdleState::IdleState(Scores *ps) : SubState<Scores>(ps), m_idle_time(0) {}

void Scores::IdleState::update(const UpdateState &updateState)
{
    Uint32 dt = updateState.delta_time;

    m_idle_time += dt;

    if (m_idle_time > AppConfig::Score::idle_time)
    {
        m_parent_state->transiteToNextState();
    }
}

void Scores::IdleState::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

        if (ev.isPressed(KeyCode::KEY_RETURN))
        {
            m_parent_state->transiteToNextState();
        }
    }
}