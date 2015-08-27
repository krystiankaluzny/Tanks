#include "app.h"
#include "game/engine/engine.h"
#include "appthread.h"
#include "game/game.h"
#include "network/network.h"
#include "shareddata.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <process.h>
#include <windows.h>
#include <vector>


void threadFunction(void* ptr)
{
    AppThread* thread = reinterpret_cast<AppThread*>(ptr);
    thread->run();

    _endthread();
}

App::App()
{
}

void App::run()
{
        std::vector < HANDLE > threads;

        CRITICAL_SECTION critical_section;                  //instancja sekcji krytycznej
        InitializeCriticalSection(&critical_section);       //inicjalizacja

        SharedData shared_data;                             //współdzielone dane
//        shared_data.network_state = NetworkState::SERVER;   //włączamy serwer //docelowo NONE

        Game* game = new Game(&shared_data, &critical_section);             //instancja gry
        Network* server = new Network(&shared_data, &critical_section);     //instancja sieci

        HANDLE game_thread = (HANDLE) _beginthread(threadFunction, 0, game);      //odpalenie wątku gry
        HANDLE network_thread = (HANDLE) _beginthread(threadFunction, 0, server); //odpalenie wątku sieci
        threads.push_back(game_thread);
        threads.push_back(network_thread);

        WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);        //czekanie na zakończenie wątków
        DeleteCriticalSection(&critical_section );                                  //usuwanie sekcji krytycznej
}

