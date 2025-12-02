#include "menu.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../appconfig.h"
#include "../spriteconfig.h"
#include "../app_state/game.h"

#include <iostream>

Menu::Menu()
{
    m_menu_items.push_back("1 Player");
    m_menu_items.push_back("2 Players");
    m_menu_items.push_back("Exit");

    m_current_menu_index = 0;

    m_menu_item_height = 32;
    m_first_menu_item_offset = {180, 120};
    m_tank_menu_pointer_offset = {144, 110};

    m_tank_menu_pointer = new Player(0, 0, ST_PLAYER_1, AppConfig::player_1_keys);
    m_tank_menu_pointer->direction = D_RIGHT;
    m_tank_menu_pointer->pos_x = 144;
    m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * 32 + 112;
    m_tank_menu_pointer->setFlag(TSF_LIFE);
    m_tank_menu_pointer->update(0);
    m_tank_menu_pointer->clearFlag(TSF_LIFE);
    m_tank_menu_pointer->clearFlag(TSF_SHIELD);
    m_tank_menu_pointer->setFlag(TSF_MENU);
    m_finished = false;
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
        text_start = {180, (i + 1) * 32 + 120};
        i++;
        renderer.drawText(text_start, text, {255, 255, 255, 255}, 2);
    }

    m_tank_menu_pointer->draw(renderer);

    renderer.flush();
}

void Menu::update(Uint32 dt)
{
    m_tank_menu_pointer->speed = m_tank_menu_pointer->default_speed;
    m_tank_menu_pointer->stop = true;
    m_tank_menu_pointer->update(dt);
}

void Menu::eventProcess(const Event &event)
{
    if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent &ev = static_cast<const KeyboardEvent &>(event);

        if (ev.isPressed(KeyCode::KEY_UP))
        {
            m_current_menu_index--;
            if (m_current_menu_index < 0)
                m_current_menu_index = m_menu_items.size() - 1;

            m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * 32 + 110;
        }
        else if (ev.isPressed(KeyCode::KEY_DOWN))
        {
            m_current_menu_index++;
            if (m_current_menu_index >= m_menu_items.size())
                m_current_menu_index = 0;

            m_tank_menu_pointer->pos_y = (m_current_menu_index + 1) * 32 + 110;
        }
        else if (ev.isPressed(KeyCode::KEY_SPACE) || ev.isPressed(KeyCode::KEY_RETURN))
        {
            m_finished = true;
        }
        else if (ev.isPressed(KeyCode::KEY_ESCAPE))
        {
            m_current_menu_index = -1;
            m_finished = true;
        }
    }
}

AppState *Menu::nextState()
{
    if (!m_finished)
        return this;

    if (m_current_menu_index == m_menu_items.size() - 1)
        return nullptr;
    else if (m_current_menu_index == 0)
    {
        Game *g = new Game(1);
        return g;
    }
    else if (m_current_menu_index == 1)
    {
        Game *g = new Game(2);
        return g;
    }
    return nullptr;
}
