#include "stage_select.h"
#include "../engine/renderer.h"
#include "../appconfig.h"
#include "../soundconfig.h"
#include "game/game.h"
#include "menu.h"

StageSelect::StageSelect(int players_count, InteractiveComponents interactive_components, StateMachine *state_machine)
    : AppState(interactive_components, state_machine),
      m_players_count(players_count),
      m_selected_stage(1)
{
}

void StageSelect::onInitialize()
{
    stopAllSounds();
    // Hack to fill sound device buffer and avoid sound delay on first play
    playSound(SoundConfig::MENU_ITEM_SELECTED);
}

void StageSelect::draw(Renderer &renderer)
{
    renderer.clear();

    renderer.drawRect(AppConfig::windows_rect, {180, 180, 180, 255}, true);

    renderer.drawText({-1, 130}, "SELECT STAGE", {0, 0, 0, 255}, FontSize::BIG);

    renderer.drawText({-1, 197}, "<      >", {0, 0, 0, 255}, FontSize::BIGGER);
    renderer.drawText({-1, 190}, std::to_string(m_selected_stage), {0, 0, 0, 255}, FontSize::BIGGEST);

    renderer.flush();
}

void StageSelect::update(const UpdateState &)
{
}

void StageSelect::eventProcess(const Event &event)
{
    if (event.type() != Event::KEYBOARD)
        return;

    const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

    if (ev.isPressed(KeyCode::KEY_LEFT) || ev.isPressed(KeyCode::KEY_UP))
    {
        m_selected_stage--;
        if (m_selected_stage < 1)
            m_selected_stage = (int)AppConfig::stages_count;
        playSound(SoundConfig::MENU_ITEM_SELECTED);
    }
    else if (ev.isPressed(KeyCode::KEY_RIGHT) || ev.isPressed(KeyCode::KEY_DOWN))
    {
        m_selected_stage++;
        if (m_selected_stage > (int)AppConfig::stages_count)
            m_selected_stage = 1;
        playSound(SoundConfig::MENU_ITEM_SELECTED);
    }
    else if (ev.isPressed(KeyCode::KEY_SPACE) || ev.isPressed(KeyCode::KEY_RETURN))
    {
        transiteTo(new Game(m_players_count, m_selected_stage, m_interactive_components, m_state_machine));
    }
    else if (ev.isPressed(KeyCode::KEY_ESCAPE))
    {
        transiteTo(new Menu(m_interactive_components, m_state_machine));
    }
}
