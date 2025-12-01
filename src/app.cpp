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
    is_running = false;
}

App::~App()
{
    if (m_app_state != nullptr)
        delete m_app_state;
}

void App::run()
{
    is_running = true;
    // inicjalizacja SDL i utworzenie okan

    SDLEngine sdl_engine;

    sdl_engine.setConfig({"Tanks", AppConfig::windows_rect});
    Engine::setEngine(&sdl_engine);

    Engine &engine = Engine::getEngine();

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
    // m_app_state->eventProcess();
    // if (m_app_state->finished())
    // {
        // AppState *next_state = m_app_state->nextState();
        // delete m_app_state;
        // m_app_state = next_state;
        // if (m_app_state == nullptr)
        // {
        //     is_running = false;
        //     return ProcessingResult::STOP;
        // }
    // }
    return ProcessingResult::CONTINUE;
}

ProcessingResult App::updateState(Uint32 delta_time)
{
    // m_app_state->update(delta_time);

    return ProcessingResult::CONTINUE;
}

ProcessingResult App::draw(Renderer &renderer)
{
    // m_app_state->draw();
    return ProcessingResult::CONTINUE;
}

void App::eventProces()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            is_running = false;
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                event.window.event == SDL_WINDOWEVENT_MAXIMIZED ||
                event.window.event == SDL_WINDOWEVENT_RESTORED ||
                event.window.event == SDL_WINDOWEVENT_SHOWN)
            {

                AppConfig::windows_rect.w = event.window.data1;
                AppConfig::windows_rect.h = event.window.data2;
                Engine::getEngine().getRenderer()->setScale((float)AppConfig::windows_rect.w / (AppConfig::map_rect.w + AppConfig::status_rect.w),
                                                            (float)AppConfig::windows_rect.h / AppConfig::map_rect.h);
            }
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11)
        {
            // Engine::getEngine().getRenderer()->toggleFullscreen(m_window);
        }
        else
        {
            m_app_state->eventProcess(&event);
        }
    }
}
