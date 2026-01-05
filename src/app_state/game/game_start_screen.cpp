#include "game.h"
#include "../../appconfig.h"

Game::StartScreenState::StartScreenState(Game *ps) : SubState(ps, ps->m_game_state_machine), m_level_start_time(0) {}

void Game::StartScreenState::draw(Renderer &renderer)
{
    m_parent_state->drawScene(renderer);

    Color color = {128, 128, 128, 0};

    double progress = (double)m_level_start_time / AppConfig::Game::level_start_time;
    int rect_height = AppConfig::map_rect.h / 2 * (1.0 - progress);

    Rect top_rect = {0, 0, AppConfig::map_rect.w, rect_height};
    renderer.drawRect(top_rect, color, true);

    Rect bottom_rect = {0, AppConfig::map_rect.h - rect_height, AppConfig::map_rect.w, rect_height};
    renderer.drawRect(bottom_rect, color, true);

    Point text_centered_pos = {-1, -1};
    std::string level_name = "STAGE " + std::to_string(m_parent_state->m_current_level);
    renderer.drawText(text_centered_pos, level_name, {255, 255, 255, 255}, FontSize::BIGGEST);
}

void Game::StartScreenState::update(const UpdateState &updateState)
{
    m_level_start_time += updateState.delta_time;

    if (m_level_start_time > AppConfig::Game::level_start_time)
    {
        transiteToNull();
    }
}
