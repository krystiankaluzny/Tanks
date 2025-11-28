#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include "../engine.h"
#include "sdl_renderer.h"


class SDLEngine : public Engine
{
private:
    bool is_main_loop_running = false;

    SDL_Window *m_window;
    Renderer* m_renderer;
    SpriteConfig* m_sprite_config;

    void initModules();
    void destroyModules();
public:
    SDLEngine();
    ~SDLEngine();

    void startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) override;
};

#endif // SDL_ENGINE_H