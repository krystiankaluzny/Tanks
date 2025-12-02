#include "app.h"
#include "appconfig.h"
#include "engine/engine.h"
#include "engine/sdl/sdl_engine.h"
#include "app_state/game.h"
#include "app_state/menu.h"

#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

App::App()
{
    m_app_state = nullptr;
}

App::~App()
{
    if (m_app_state != nullptr)
        delete m_app_state;
}

void App::run()
{

    SDLEngine sdl_engine;

    sdl_engine.setConfig({"Tanks", Size{AppConfig::windows_rect.w, AppConfig::windows_rect.h}});

    Engine &engine = sdl_engine;

    m_app_state = new Menu();

    engine.startMainLoop(
        [&](const Event &event)
        { return handleEvent(event); },
        [&](Uint32 dt)
        { return updateState(dt); },
        [&](Renderer &renderer)
        { return draw(renderer); });
}

ProcessingResult App::handleEvent(const Event &event)
{
    AppState *next_state = m_app_state->nextState();
    if (m_app_state != next_state)
    {
        delete m_app_state;
        m_app_state = next_state;
        if (m_app_state == nullptr)
        {
            return ProcessingResult::STOP;
        }
    }

    m_app_state->eventProcess(event);
    return ProcessingResult::CONTINUE;
}

ProcessingResult App::updateState(Uint32 delta_time)
{
    m_app_state->update(delta_time);
    return ProcessingResult::CONTINUE;
}

ProcessingResult App::draw(Renderer &renderer)
{
    m_app_state->draw(renderer);
    return ProcessingResult::CONTINUE;
}
