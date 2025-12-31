#ifndef ENGINE_H
#define ENGINE_H

#include "data/event.h"
#include "renderer.h"
#include "sound_manager.h"
#include "interactive_component.h"
#include <functional>



struct UpdateState
{
    Uint32 delta_time;
};

enum class ProcessingResult
{
    STOP,
    CONTINUE
};

class Engine;

typedef std::function<ProcessingResult(const Engine &)> OnEngineInitFunc;
typedef std::function<ProcessingResult(const Event &)> HandleEventFunc;
typedef std::function<ProcessingResult(const UpdateState &)> UpdateStateFunc;
typedef std::function<ProcessingResult(Renderer &)> DrawFunc;

class Engine
{
public:
    Engine() {};

    virtual InteractiveComponents getInteractiveComponents() const = 0;
    virtual void startMainLoop(OnEngineInitFunc onEngineInit, HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) = 0;
};

#endif // ENGINE_H
