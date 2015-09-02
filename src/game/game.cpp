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

//#define RELEASE

Game::Game(SharedData *shared_data, CRITICAL_SECTION *critical_section) :
    AppThread(shared_data, critical_section)
{
    m_window = nullptr;
}

Game::~Game()
{
    if(m_game_state != nullptr)
        delete m_game_state;
}

void Game::run()
{
    if(initSDL())
    {
        Engine::getEngine().initModules();

        is_running = true;

        //utworzenie okna
        m_window = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    AppConfig::windows_rect.w, AppConfig::windows_rect.h,
                            #ifdef RELEASE
                                    SDL_WINDOW_FULLSCREEN_DESKTOP
                            #else
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                            #endif
                            );

        if(m_window == nullptr) return;

        Engine::getEngine().getRenderer()->loadTexture(m_window);
        Engine::getEngine().getRenderer()->loadFont();

        srand(time(NULL)); //inicjowanie generatora pseudolosowego

        m_game_state = new Menu(this);

        //========pętla główna========
        mainLoop();
        //========pętla główna========

        EnterCriticalSection(critical_section);
            shared_data->run_app = false;
        LeaveCriticalSection(critical_section);

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        Engine::getEngine().destroyModules();
        quitSDL();
    }
}

void Game::eventProces()
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

        m_game_state->eventProcess(&event);
    }

    networkEvent();
}

void Game::networkEvent()
{
    if(!isNetworkRunning()) return;

    EventsWrapper events;
//    std::cout << "Start game 2.1" << std::endl;
    unsigned long current_frame = getCurrentFrame();
//    std::cout << "Start game 2.2" << std::endl;
    EnterCriticalSection(critical_section);
        events = shared_data->received_events.frame_events[current_frame];
    LeaveCriticalSection(critical_section);

//    std::cout << "Start game 2.3" << std::endl;
    m_game_state->eventProcess(events);
}

void Game::mainLoop()
{
    double FPS;
    Uint32 time1, time2, dt, fps_time = 0, fps_count = 0, delay = 15;
    time1 = SDL_GetTicks();
    while(is_running)
    {
        time2 = SDL_GetTicks();
        dt = time2 - time1;
        time1 = time2;

        if(m_game_state->finished())
        {
            GameState* new_state = m_game_state->nextState();
            delete m_game_state;
            m_game_state = new_state;
        }
        if(m_game_state == nullptr) break;

        eventProces();

        m_game_state->update(AppConfig::game_speed);
        m_game_state->draw();

        SDL_Delay(delay);

        //FPS
        fps_time += dt;
        fps_count++;
        if(fps_time > 200)
        {
            FPS = (double)fps_count / fps_time * 1000;
            if(FPS > 60) delay++;
            else if(delay > 0) delay--;
            fps_time = 0; fps_count = 0;
        }

        NetworkState state;
        EnterCriticalSection(critical_section);
            shared_data->incrementFrameNumber();
            state = shared_data->network_state;
        LeaveCriticalSection(critical_section);

//        if(state != NetworkState::NONE)
//        {
//            std::cout << "Current frame: " << getCurrentFrame() << std::endl;
//        }
        Sleep( 0 ); // reszta czasu dla drugiego wątku
    }
}

bool Game::initSDL()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if(TTF_Init() == -1) return false;
    return true;
}

void Game::quitSDL()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
