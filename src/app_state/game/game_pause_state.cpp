#include "game.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include "../menu.h"

Game::PauseState::PauseState(Game *ps) : SubState(ps, ps->m_game_state_machine)
{
    m_parent_state->stopAllSounds();
    m_parent_state->playSound(SoundConfig::PAUSE);
}

void Game::PauseState::draw(Renderer &renderer)
{
    m_parent_state->drawScene(renderer);

    Point text_centered_pos = {-1, -1};
    renderer.drawText(text_centered_pos, std::string("PAUSE"), {200, 0, 0, 255}, FontSize::BIGGEST);
}

void Game::PauseState::update(const UpdateState &updateState)
{
}

void Game::PauseState::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &event_key = static_cast<const KeyboardEvent &>(event);

        if (event_key.isPressed(KEY_RETURN))
        {
            transiteTo(new Game::PlayingState(m_parent_state));
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            m_parent_state->transiteTo(new Menu(m_parent_state->m_interactive_components, m_parent_state->m_state_machine));
        }
    }
}