#include "game.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include "../menu.h"

Game::PauseState::PauseState(Game *ps) : ContextState(ps, ps->m_game_state_machine)
{
}

void Game::PauseState::onInitialize()
{
    m_context->stopAllSounds();
    m_context->playSound(SoundConfig::PAUSE);
}

void Game::PauseState::draw(Renderer &renderer)
{
    m_context->drawScene(renderer);

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
            transiteTo(new Game::PlayingState(m_context));
        }
        else if (event_key.isPressed(KEY_ESCAPE))
        {
            m_context->transiteTo(new Menu(m_context->m_interactive_components, m_context->m_state_machine));
        }
    }
}