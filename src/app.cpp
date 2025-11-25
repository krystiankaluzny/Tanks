#include "app.h"
#include "appconfig.h"
#include "engine/engine.h"
#include "app_state/game.h"
#include "app_state/menu.h"

#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define VERSION "1.0.0"

App::App()
{
    m_window = nullptr;
    m_app_state = nullptr;
    is_running = false;
}

App::~App()
{
    if(m_app_state != nullptr)
        delete m_app_state;
}

void App::run()
{
    is_running = true;
    //inicjalizacja SDL i utworzenie okan

    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        m_window = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    AppConfig::windows_rect.w, AppConfig::windows_rect.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(m_window == nullptr) return;

        if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return;
        if(TTF_Init() == -1) return;

        std::random_device dev;
        srand(dev());

        Engine& engine = Engine::getEngine();
        engine.initModules();
        engine.getRenderer()->loadTexture(m_window);
        engine.getRenderer()->loadFont();

        m_app_state = new Menu;

        Uint32 fps_time = 0, fps_count = 0, delay = 15;
        Uint32 currentTime = SDL_GetTicks();
        Uint32 previousTime = currentTime;
        while(is_running)
        {
            currentTime = SDL_GetTicks();
            Uint32 deltaTime = currentTime - previousTime;
            previousTime = currentTime;

            if(m_app_state->finished())
            {
                AppState* new_state = m_app_state->nextState();
                delete m_app_state;
                m_app_state = new_state;
            }
            if(m_app_state == nullptr) break;

            eventProces();

            m_app_state->update(deltaTime);
            m_app_state->draw();

            SDL_Delay(delay);

            //FPS
            fps_time += deltaTime; fps_count++;
            if(fps_time > 200)
            {
                const double fps = static_cast<double>(fps_count) / fps_time * 1000;
                if(fps > 60) delay++;
                else if(delay > 0) delay--;
                fps_time = 0; fps_count = 0;
            }
        }

        engine.destroyModules();
    }

    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void App::eventProces()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            is_running = false;
        }
        else if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED ||
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
        else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11)
        {
            Engine::getEngine().getRenderer()->toggleFullscreen(m_window);
        }
        else
        {
            m_app_state->eventProcess(&event);
        }
    }
}
