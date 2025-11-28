#include "sdl_engine.h"

#include <random>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDLEngine::SDLEngine()
{
}

SDLEngine::~SDLEngine()
{
}

void SDLEngine::initModules()
{
    m_renderer = new SDLRenderer;
    m_sprite_config = new SpriteConfig;
}

void SDLEngine::destroyModules()
{
    delete m_renderer;
    m_renderer = nullptr;
    delete m_sprite_config;
    m_sprite_config = nullptr;
}

void SDLEngine::startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw)
{
    is_main_loop_running = true;

    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        m_window = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    AppConfig::windows_rect.w, AppConfig::windows_rect.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(m_window == nullptr) return;

        if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return;
        if(TTF_Init() == -1) return;

        std::random_device dev;
        srand(dev());

        initModules();
        m_renderer->loadTexture(m_window);
        m_renderer->loadFont();



        Uint32 probe_time = 0, frames_count = 0, delay = 15;
        Uint32 current_time = SDL_GetTicks();
        Uint32 previous_time = current_time;
        while(is_main_loop_running)
        {
            current_time = SDL_GetTicks();
            Uint32 delta_time = current_time - previous_time;
            previous_time = current_time;

            handleEvent();
            updateState(delta_time);
            draw();

            SDL_Delay(delay);

            //FPS
            probe_time += delta_time;
            frames_count++;
            if(probe_time > 200)
            {
                const double fps = static_cast<double>(frames_count) / probe_time * 1000;
                if(fps > 60) delay++;
                else if(delay > 0) delay--;
                probe_time = 0;
                frames_count = 0;
            }
        }

        destroyModules();

        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    
}