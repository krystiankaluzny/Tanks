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

        CRITICAL_SECTION critical_section;
        InitializeCriticalSection(&critical_section);

        SharedData shared_data;
        shared_data.network_state = NetworkState::SERVER;

        Game* game = new Game(&shared_data, &critical_section);
        Network* server = new Network(&shared_data, &critical_section);

        HANDLE game_thread = (HANDLE) _beginthread( threadFunction, 0, game );
        HANDLE network_thread = (HANDLE) _beginthread( threadFunction, 0, server );
        threads.push_back(game_thread);
        threads.push_back(network_thread);

        WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);
        DeleteCriticalSection(&critical_section );
}

bool App::initSDL()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if(TTF_Init() == -1) return false;
    return true;
}

void App::quitSDL()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
