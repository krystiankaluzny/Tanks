#include "menu.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../appconfig.h"
#include "../soundconfig.h"
#include "../spriteconfig.h"
#include "game/game.h"

#include <iostream>

Menu::Menu(InteractiveComponents interactive_components, StateMachine *state_machine)
    : AppState(interactive_components, state_machine)
{
    m_menu_items.push_back("1 Player");
    m_menu_items.push_back("2 Players");
    m_menu_items.push_back("Exit");

    m_current_menu_index = 0;

    m_menu_item_height = AppConfig::tile_size.h * 2;
    m_first_menu_item_offset = {180, 120};
    m_tank_menu_pointer_offset = {140, 110};

    m_tank_menu_pointer = new Player(0, 0, ST_PLAYER_1, AppConfig::player_1_keys, m_interactive_components);
    m_tank_menu_pointer->setDirection(D_RIGHT);
    m_tank_menu_pointer->pos_x = m_tank_menu_pointer_offset.x;
    m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * m_menu_item_height + m_tank_menu_pointer_offset.y;
    m_tank_menu_pointer->setFlag(Tank::TSF_ALIVE);
    m_tank_menu_pointer->update(0);
    m_tank_menu_pointer->clearFlag(Tank::TSF_ALIVE);
    m_tank_menu_pointer->clearFlag(Tank::TSF_SHIELD);
    m_tank_menu_pointer->setFlag(Tank::TSF_PREVIEW);

    stopAllSounds();
    // Hack to fill sound device buffer and avoid sound delay on first play
    playSound(SoundConfig::MENU_ITEM_SELECTED);
}

Menu::~Menu()
{
    delete m_tank_menu_pointer;
}

void Menu::draw(Renderer &renderer)
{
    renderer.clear();

    renderer.drawRect(AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer.drawRect(AppConfig::status_rect, {0, 0, 0, 255}, true);

    // LOGO
    const SpriteData &logo = SpriteConfig::getInstance().getSpriteData(ST_TANKS_LOGO);
    Rect dst = {(AppConfig::map_rect.w + AppConfig::status_rect.w - logo.rect.w) / 2, 50, logo.rect.w, logo.rect.h};
    renderer.drawObject(logo.rect, dst);

    int i = 0;
    Point text_start;
    for (auto text : m_menu_items)
    {
        text_start = {m_first_menu_item_offset.x, (i + 1) * m_menu_item_height + m_first_menu_item_offset.y};
        i++;
        renderer.drawText(text_start, text, {255, 255, 255, 255}, FontSize::BIG);
    }

    m_tank_menu_pointer->draw(renderer);

    renderer.flush();
}

void Menu::update(const UpdateState &updateState)
{
    m_tank_menu_pointer->update(updateState.delta_time);
}

void Menu::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

        if (ev.isPressed(KeyCode::KEY_UP))
        {
            if (m_current_menu_index == 0)
            {
                m_current_menu_index = (int)m_menu_items.size() - 1;
            }
            else
            {
                m_current_menu_index--;
            }

            m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * m_menu_item_height + m_tank_menu_pointer_offset.y;
            playSound(SoundConfig::MENU_ITEM_SELECTED);
        }
        else if (ev.isPressed(KeyCode::KEY_DOWN))
        {

            if (m_current_menu_index >= (int)m_menu_items.size() - 1)
            {
                m_current_menu_index = 0;
            }
            else
            {
                m_current_menu_index++;
            }

            m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * m_menu_item_height + m_tank_menu_pointer_offset.y;
            playSound(SoundConfig::MENU_ITEM_SELECTED);
        }
        else if (ev.isPressed(KeyCode::KEY_SPACE) || ev.isPressed(KeyCode::KEY_RETURN))
        {
            if (m_current_menu_index == 0 || m_current_menu_index == 1)
            {
                int players_count = m_current_menu_index == 0 ? 1 : 2;
                transiteTo(new Game(players_count, m_interactive_components, m_state_machine));
            }
            else
            {
                transiteToNullAndStop();
            }
        }
        else if (ev.isPressed(KeyCode::KEY_ESCAPE))
        {
            transiteToNullAndStop();
        }
    }
}