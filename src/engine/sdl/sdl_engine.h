#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include "../engine.h"
#include "sdl_renderer.h"

struct SDLEngineConfig
{
    std::string window_title;
    Size initial_window_size;
};

class SDLEngine : public Engine
{
private:
    bool is_main_loop_running = false;

    SDL_Window *m_window;
    SDLRenderer *m_renderer;
    SpriteConfig *m_sprite_config;
    SDLEngineConfig m_config;

    void initModules();
    void destroyModules();

    ProcessingResult handleEvents(HandleEventFunc handleEvent);
    ProcessingResult handleInternalEvents(Event event);

public:
    ~SDLEngine();
    void startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) override;

    void setConfig(SDLEngineConfig config);

    Renderer *getRenderer() const override;
    SpriteConfig *getSpriteConfig() const override;
};

#endif // SDL_ENGINE_H