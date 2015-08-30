#include "graphicthread.h"

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

GraphicThread::GraphicThread(SharedData *shared_data, CRITICAL_SECTION *critical_section) :
    AppThread(shared_data, critical_section)
{
     m_window = nullptr;
}

void GraphicThread::run()
{
    if(initSDL())
    {
        Engine::getEngine().initModules();

        is_running = true;

        //utworzenie okna
        m_window = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    AppConfig::windows_rect.w, AppConfig::windows_rect.h, SDL_WINDOW_FULLSCREEN);

        if(m_window == nullptr) return;

        Engine::getEngine().getRenderer()->loadTexture(m_window);
        Engine::getEngine().getRenderer()->loadFont();

        //TODO usunąć
        srand(time(NULL)); //inicjowanie generatora pseudolosowego

        //========pętla główna========
        mainLoop();
        //========pętla główna========

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        Engine::getEngine().destroyModules();
        quitSDL();
    }
}

void GraphicThread::mainLoop()
{
    while(is_running)
    {
        Engine& engine = Engine::getEngine();
        Renderer* renderer = engine.getRenderer();
        renderer->clear();

        renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 0}, true);

        renderer->flush();
//        EnterCriticalSection(critical_section);
//            is_running = shared_data->run_app;
//        LeaveCriticalSection(critical_section);

//        bool get_obj = false;
//        Object* obj;
//        do
//        {
//            EnterCriticalSection(critical_section);
//                get_obj = shared_data->object_to_render.pop(&obj);
//            LeaveCriticalSection(critical_section);

//            if(!get_obj) break;

//            EnterCriticalSection(critical_section);
//                obj->draw();
//            LeaveCriticalSection(critical_section);
//        }while(true);

        eventProcess();

        Sleep( 5 ); // reszta czasu dla drugiego wątku
    }
}

bool GraphicThread::initSDL()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if(TTF_Init() == -1) return false;
    return true;
}

void GraphicThread::quitSDL()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GraphicThread::eventProcess()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        EnterCriticalSection(critical_section);
           shared_data->sdl_events_queue.push(event);
        LeaveCriticalSection(critical_section);
    }
}
