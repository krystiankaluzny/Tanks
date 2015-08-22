#include "app.h"
#include "game/engine/engine.h"
#include "appthread.h"
#include "game/game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>

int threadFunction(void* ptr)
{
    AppThread* thread = reinterpret_cast<AppThread*>(ptr);
    thread->run();
    return 0;
}

App::App()
{

}

void App::run()
{
    if(initSDL())
    {
        Engine::getEngine().initModules();

        Game* game = new Game();
        SDL_Thread* game_thread = SDL_CreateThread(threadFunction, "Game", game);

        SDL_WaitThread(game_thread, nullptr);

        Engine::getEngine().destroyModules();
        quitSDL();
    }
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
