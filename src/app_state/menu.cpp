#include "menu.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../appconfig.h"
#include "../type.h"
#include "../app_state/game.h"

#include <iostream>

Menu::Menu()
{
    m_menu_texts.push_back("1 Player");
    m_menu_texts.push_back("2 Players");
    m_menu_texts.push_back("Exit");
    m_menu_index = 0;
    m_tank_pointer = new Player(0, 0 , ST_PLAYER_1);
    m_tank_pointer->direction = D_RIGHT;
    m_tank_pointer->pos_x = 144;
    m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 112;
    m_tank_pointer->setFlag(TSF_LIFE);
    m_tank_pointer->update(0);
    m_tank_pointer->clearFlag(TSF_LIFE);
    m_tank_pointer->clearFlag(TSF_SHIELD);
    m_tank_pointer->setFlag(TSF_MENU);
    m_finished = false;
}

Menu::~Menu()
{
    delete m_tank_pointer;
}

void Menu::draw()
{
    Renderer* renderer = Engine::getEngine().getRenderer();
    renderer->clear();

    renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer->drawRect(&AppConfig::status_rect, {0, 0, 0, 255}, true);

    //LOGO
    const SpriteData* logo = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_TANKS_LOGO);
    SDL_Rect dst = {(AppConfig::map_rect.w + AppConfig::status_rect.w - logo->rect.w)/2, 50, logo->rect.w, logo->rect.h};
    renderer->drawObject(&logo->rect, &dst);

    int i = 0;
    SDL_Point text_start;
    for(auto text : m_menu_texts)
    {
        text_start = { 180, (i + 1) * 32 + 120};
        i++;
        renderer->drawText(&text_start, text, {255, 255, 255, 255}, 2);
    }

    m_tank_pointer->draw();

    renderer->flush();
}

void Menu::update(Uint32 dt)
{
    m_tank_pointer->speed = m_tank_pointer->default_speed;
    m_tank_pointer->stop = true;
    m_tank_pointer->update(dt);
}

void Menu::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_UP)
        {
            m_menu_index--;
            if(m_menu_index < 0)
                m_menu_index = m_menu_texts.size() - 1;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 110;
        }
        else if(ev->key.keysym.sym == SDLK_DOWN)
        {
            m_menu_index++;
            if(m_menu_index >= m_menu_texts.size())
                m_menu_index = 0;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 110;
        }
        else if(ev->key.keysym.sym == SDLK_SPACE || ev->key.keysym.sym == SDLK_RETURN)
        {
            m_finished = true;
        }
        else if(ev->key.keysym.sym == SDLK_ESCAPE)
        {
            m_menu_index = -1;
            m_finished = true;
        }
    }
}

bool Menu::finished() const
{
    return m_finished;
}

AppState* Menu::nextState()
{
    if(m_menu_index == m_menu_texts.size() - 1)
        return nullptr;
    else if(m_menu_index == 0)
    {
        Game* g = new Game(1);
        return g;
    }
    else if(m_menu_index == 1)
    {
        Game* g = new Game(2);
        return g;
    }
    return nullptr;
}
