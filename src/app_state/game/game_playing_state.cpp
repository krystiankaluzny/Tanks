#include "game.h"
#include "../../appconfig.h"
#include "../menu.h"

Game::PlayingState::PlayingState(Game *ps) : ContextState(ps, ps->m_game_state_machine) {}

void Game::PlayingState::draw(Renderer &renderer)
{
    m_context->drawScene(renderer);
}

void Game::PlayingState::update(const UpdateState &updateState)
{
    m_context->updateScene(updateState.delta_time);

    if (m_context->m_enemies.empty() && m_context->m_enemies_to_kill_count <= 0)
    {
        transiteTo(new Game::LevelEndingState(m_context, false, false));
    }

    if (m_context->m_players.empty())
    {
        transiteTo(new Game::GameOverState(m_context));
    }
}

void Game::PlayingState::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &event_key = static_cast<const KeyboardEvent &>(event);
        if (event_key.isPressed(KEY_N))
        {
            transiteTo(new Game::LevelEndingState(m_context, true, false));
        }
        else if (event_key.isPressed(KEY_B))
        {
            m_context->m_current_level -= 2;
            transiteTo(new Game::LevelEndingState(m_context, true, false));
        }
        else if (event_key.isPressed(KEY_T))
        {
            m_context->m_show_enemies_targets = !m_context->m_show_enemies_targets;
        }
        else if (event_key.isPressed(KEY_RETURN))
        {
            transiteTo(new Game::PauseState(m_context));
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            m_context->transiteTo(new Menu(m_context->m_interactive_components, m_context->m_state_machine));
        }

         for (auto player : m_context->m_players)
                player->handleKeyboardEvent(event_key);
    }
}