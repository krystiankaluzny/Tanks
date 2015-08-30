#include "game.h"
#include "../appconfig.h"
#include "engine/engine.h"
#include "game_state/battle.h"
#include "game_state/menu.h"
#include "game_state/gamestate.h"

#include <ctime>
#include <iostream>
#include <stdlib.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <windows.h>

Game::Game(SharedData *shared_data, CRITICAL_SECTION *critical_section) :
    AppThread(shared_data, critical_section)
{
//    Menu* dupa = new Menu(this);
    m_window = nullptr;
}

Game::~Game()
{
    if(m_game_state != nullptr)
        delete m_game_state;
}

void Game::run()
{
    is_running = true;

    EnterCriticalSection(critical_section);
        m_game_state = nullptr;
    LeaveCriticalSection(critical_section);

    mainLoop();
}

void Game::eventProces()
{
    SDL_Event event;
    bool result = false;
    do
    {
//        EnterCriticalSection(critical_section);
//            result = shared_data->sdl_events_queue.pop(&event);
//        LeaveCriticalSection(critical_section);


        if(!result) break;
        std::cout << "Run2 " << std::endl;

        if(event.type == SDL_QUIT)
        {
            is_running = false;

            EnterCriticalSection(critical_section);
                shared_data->run_app = false;
            LeaveCriticalSection(critical_section);

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
        std::cout << "pop Event " << std::endl;

        m_game_state->eventProcess(&event);

    }
    while(true);

    networkEvent();
}

void Game::networkEvent()
{
    if(!isNetworkRunning()) return;

    EventsWrapper events;
    unsigned long current_frame = getCurrentFrame();
    EnterCriticalSection(critical_section);
        events = shared_data->received_events.frame_events[current_frame];
    LeaveCriticalSection(critical_section);

    if(events.player_id_events.size())
        std::cout << events.player_id_events.size() << " " << current_frame << std::endl;
    m_game_state->eventProcess(events);
}

void Game::mainLoop()
{
    double FPS;
    Uint32 time1, time2, dt, fps_time = 0, fps_count = 0, delay = 15;
    time1 = SDL_GetTicks();
    while(is_running)
    {
        EnterCriticalSection(critical_section);
            is_running = shared_data->run_app;
        LeaveCriticalSection(critical_section);

        time2 = SDL_GetTicks();
        dt = time2 - time1;
        time1 = time2;

        std::cout << "DUPA " << time2 << std::endl;
        std::cout << m_game_state << std::endl;

//        if(m_game_state->finished())
        {
//            GameState* new_state = m_game_state->nextState();
//            delete m_game_state;
//            m_game_state = new_state;
        }
//        if(m_game_state == nullptr) break;

//        eventProces();

//        m_game_state->updateState(AppConfig::game_speed);
//        m_game_state->draw();

        SDL_Delay(delay);

        //FPS
//        fps_time += dt;
//        fps_count++;
//        if(fps_time > 200)
//        {
////            std::cout << "Run " << dt << " "  << delay<< std::endl;
//            FPS = (double)fps_count / fps_time * 1000;
//            if(FPS > 60) delay++;
//            else if(delay > 1) delay--;
//            fps_time = 0; fps_count = 0;
//        }
//        shared_data->incrementFrameNumber();

        Sleep( 15 ); // reszta czasu dla drugiego wątku
    }
}

