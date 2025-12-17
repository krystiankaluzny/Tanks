#ifndef ENGINE_H
#define ENGINE_H

#include "data/event.h"
#include "renderer.h"
#include <functional>


enum class ProcessingResult
{
    STOP,
    CONTINUE
};

typedef std::function<ProcessingResult (const Event&)> HandleEventFunc;
typedef std::function<ProcessingResult (Uint32)> UpdateStateFunc;
typedef std::function<ProcessingResult (Renderer&)> DrawFunc;

class Engine
{
public:
    Engine() {};

    virtual void startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) = 0;

    virtual Renderer* getRenderer() const = 0;
};

#endif // ENGINE_H
