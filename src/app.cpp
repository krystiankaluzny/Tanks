#include "app.h"
#include "appconfig.h"
#include "engine/engine.h"
#include "engine/sdl/sdl_engine.h"
#include "app_state/menu.h"

#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

App::App()
{
    m_app_state_machine = new StateMachine;
}

App::~App()
{
    if (m_app_state_machine != nullptr)
        delete m_app_state_machine;
}

void App::run()
{

    SDLEngine sdl_engine;

    sdl_engine.setConfig({"Tanks", Size{AppConfig::windows_rect.w, AppConfig::windows_rect.h}});

    Engine &engine = sdl_engine;

    engine.startMainLoop(
        [&](const Engine &engine)
        { return onEngineInit(engine); },
        [&](const Event &event)
        { return handleEvent(event); },
        [&](const UpdateState &us)
        { return updateState(us); },
        [&](Renderer &renderer)
        { return draw(renderer); });
}

ProcessingResult App::onEngineInit(const Engine &engine)
{
    InteractiveComponents components = engine.getInteractiveComponents();

    m_app_state_machine->setState(new Menu(components, m_app_state_machine));

    return ProcessingResult::CONTINUE;
}

ProcessingResult App::handleEvent(const Event &event)
{
    m_app_state_machine->eventProcess(event);
    return ProcessingResult::CONTINUE;
}

ProcessingResult App::updateState(const UpdateState &updateState)
{
    if (m_app_state_machine->stopped())
    {
        return ProcessingResult::STOP;
    }
    m_app_state_machine->update(updateState);
    return ProcessingResult::CONTINUE;
}

ProcessingResult App::draw(Renderer &renderer)
{
    m_app_state_machine->draw(renderer);
    return ProcessingResult::CONTINUE;
}
