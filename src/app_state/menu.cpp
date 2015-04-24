#include "menu.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../appconfig.h"
#include "../type.h"

#include <iostream>

Menu::Menu()
{
    m_menu_texts.push_back("1 Player");
    m_menu_texts.push_back("2 Players");
    m_menu_texts.push_back("Exit");
    m_menu_index = 0;
    m_tank_pointer = new Player;
    m_tank_pointer->direction = D_RIGHT;
    m_tank_pointer->pos_x = 140;
    m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 110;
    m_tank_pointer->setFlag(TSF_LIFE);
    m_tank_pointer->update(0);
    m_tank_pointer->clearFlag(TSF_LIFE);
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

    int i = 0;
    SDL_Point text_start;
    for(auto text : m_menu_texts)
    {
        text_start = { 180, (i + 1) * 32 + 120};
        i++;
        renderer->drawText(&text_start, text, {255, 255, 255, 255}, true);
    }

    m_tank_pointer->draw();

    renderer->flush();
}

void Menu::update(Uint32 dt)
{
    m_tank_pointer->update(dt);
}

void Menu::eventProcess(SDL_Event *ev)
{
//    switch(ev->type)
//    {
//    case SDL_KEYDOWN:
//        switch(ev->key.keysym.sym)
//        {
//        case SDLK_UP:
//            m_players.at(0)->respawn();
//            break;
//        case SDLK_DOWN:
//            nextLevel();
//            break;

//        }
//        break;
//    }
}

bool Menu::finished() const
{
    return false;
}

AppState* Menu::nextState()
{

}
