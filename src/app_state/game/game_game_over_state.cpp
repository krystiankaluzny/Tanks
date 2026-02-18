#include "game.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include "../menu.h"

Game::GameOverState::GameOverState(Game *ps) : ContextState(ps, ps->m_game_state_machine)
{
    m_game_over_message_position = AppConfig::map_rect.h;
}

void Game::GameOverState::onInitialize()
{
    m_context->m_level_environment->destroyEagle();
    m_context->stopSound(SoundConfig::PLAYER_IDLE);
    m_context->stopSound(SoundConfig::PLAYER_MOVING);
    m_context->playSound(SoundConfig::GAME_OVER);
}

void Game::GameOverState::draw(Renderer &renderer)
{
    m_context->drawScene(renderer);

    Point pos = {-1, m_game_over_message_position};
    renderer.drawText(pos, AppConfig::game_over_text, {255, 10, 10, 255}, FontSize::BIGGEST);
}

void Game::GameOverState::update(const UpdateState &updateState)
{
    m_game_over_message_position -= AppConfig::Game::game_over_message_speed * updateState.delta_time;

    if (m_game_over_message_position < 10)
    {
        transiteTo(new Game::LevelEndingState(m_context, true, true));
    }
}

void Game::GameOverState::eventProcess(const Event &event)
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