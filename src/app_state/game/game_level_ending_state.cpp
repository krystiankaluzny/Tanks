#include "game.h"
#include "../../appconfig.h"
#include "../scores.h"

Game::LevelEndingState::LevelEndingState(Game *ps, bool no_wating, bool game_over)
    : ContextState(ps, ps->m_game_state_machine),
      m_level_end_time(0),
      m_no_waiting(no_wating),
      m_game_over(game_over)
{
}

void Game::LevelEndingState::draw(Renderer &renderer)
{
    m_context->drawScene(renderer);
}

void Game::LevelEndingState::update(const UpdateState &updateState)
{
    m_level_end_time += updateState.delta_time;

    if (m_no_waiting || m_level_end_time > AppConfig::Game::level_start_time)
    {
        transiteToNull();

        std::vector<Player *> players;

        players.insert(std::end(players), std::begin(m_context->m_players), std::end(m_context->m_players));
        players.insert(std::end(players), std::begin(m_context->m_killed_players), std::end(m_context->m_killed_players));

        m_context->m_players.clear();
        m_context->m_killed_players.clear();

        m_context->transiteTo(new Scores(players,
                                              m_context->m_current_level,
                                              m_game_over,
                                              m_context->m_interactive_components,
                                              m_context->m_state_machine));
    }
}
