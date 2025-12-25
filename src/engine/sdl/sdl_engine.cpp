#include "sdl_engine.h"
#include "sdl_event_mapper.h"

#include <random>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

SDLEngine::~SDLEngine()
{
}

void SDLEngine::initModules()
{
    m_renderer = new SDLRenderer(m_config.initial_window_size);
}

void SDLEngine::destroyModules()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void SDLEngine::startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw)
{
    is_main_loop_running = true;

    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        m_window = SDL_CreateWindow(m_config.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    m_config.initial_window_size.w, m_config.initial_window_size.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (m_window == nullptr)
            return;

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
            return;
        if (TTF_Init() == -1)
            return;

        std::random_device dev;
        srand(dev());

        initModules();
        m_renderer->loadTexture(m_window);
        m_renderer->loadFont();

        Uint32 probe_time = 0, frames_count = 0, delay = 15;
        Uint32 current_time = SDL_GetTicks();
        Uint32 previous_time = current_time;
        while (is_main_loop_running)
        {
            current_time = SDL_GetTicks();
            Uint32 delta_time = current_time - previous_time;
            previous_time = current_time;

            if (handleEvents(handleEvent) == ProcessingResult::STOP)
            {
                is_main_loop_running = false;
                break;
            }

            if (updateState(delta_time) == ProcessingResult::STOP)
            {
                is_main_loop_running = false;
                break;
            }

            if (draw(*m_renderer) == ProcessingResult::STOP)
            {
                is_main_loop_running = false;
                break;
            }

            SDL_Delay(delay);

            // FPS
            probe_time += delta_time;
            frames_count++;
            if (probe_time > 200)
            {
                const double fps = static_cast<double>(frames_count) / probe_time * 1000;
                if (fps > 60)
                    delay++;
                else if (delay > 0)
                    delay--;
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

ProcessingResult SDLEngine::handleEvents(HandleEventFunc handleEvent)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        Event* e = mapSDLEventToEngineEvent(event);
        if (event.type == SDL_QUIT)
        {
            return ProcessingResult::STOP;
        }
        else
        {
            if (handleInternalEvents(*e) == ProcessingResult::STOP)
            {
                return ProcessingResult::STOP;
            }
            if (handleEvent(*e) == ProcessingResult::STOP)
            {
                return ProcessingResult::STOP;
            }
        }

        delete e;
    }
    return ProcessingResult::CONTINUE;
}

ProcessingResult SDLEngine::handleInternalEvents(const Event& event)
{
    if (event.type() == Event::WINDOW)
    {
        const WindowEvent& we = static_cast<const WindowEvent &>(event);
        if (we.windowEventType() == WindowEvent::RESIZED ||
            we.windowEventType() == WindowEvent::MAXIMIZED ||
            we.windowEventType() == WindowEvent::RESTORED ||
            we.windowEventType() == WindowEvent::SHOWN)
        {
            m_renderer->setViewportForWindowSize(we.windowSize());
        }
    }
    else if (event.type() == Event::KEYBOARD)
    {
        const KeyboardEvent& ke = static_cast<const KeyboardEvent &>(event);
        if (ke.isPressed(KeyCode::KEY_F11))
        {
            m_renderer->toggleFullscreen(m_window);
        }
    }

    return ProcessingResult::CONTINUE;
}

void SDLEngine::setConfig(SDLEngineConfig config)
{
    m_config = config;
}