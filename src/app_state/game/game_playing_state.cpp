#include "game.h"
#include "../../appconfig.h"
#include "../menu.h"

Game::PlayingState::PlayingState(Game *ps) : SubState(ps, ps->m_game_state_machine) {}

void Game::PlayingState::draw(Renderer &renderer)
{
    m_parent_state->drawScene(renderer);
}

void Game::PlayingState::update(const UpdateState &updateState)
{
    m_parent_state->updateScene(updateState.delta_time);

    if (m_parent_state->m_enemies.empty() && m_parent_state->m_enemies_to_kill_count <= 0)
    {
        transiteTo(new Game::LevelEndingState(m_parent_state, false, false));
    }

    if (m_parent_state->m_players.empty())
    {
        transiteTo(new Game::GameOverState(m_parent_state));
    }
}

void Game::PlayingState::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &event_key = static_cast<const KeyboardEvent &>(event);
        if (event_key.isPressed(KEY_N))
        {
            transiteTo(new Game::LevelEndingState(m_parent_state, true, false));
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_parent_state->m_current_level -= 2;
            transiteTo(new Game::LevelEndingState(m_parent_state, true, false));
        }
        else if (event_key.isPressed(KEY_T))
        {
            m_parent_state->m_show_enemies_targets = !m_parent_state->m_show_enemies_targets;
        }
        else if (event_key.isPressed(KEY_RETURN))
        {
            transiteTo(new Game::PauseState(m_parent_state));
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            m_parent_state->transiteTo(new Menu(m_parent_state->m_interactive_components, m_parent_state->m_state_machine));
        }

         for (auto player : m_parent_state->m_players)
                player->handleKeyboardEvent(event_key);
    }
}