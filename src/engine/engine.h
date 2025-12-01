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
typedef std::function<ProcessingResult (int)> UpdateStateFunc;
typedef std::function<ProcessingResult (Renderer&)> DrawFunc;

class Engine
{
public:
    Engine() {};

    virtual void startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) = 0;

    static Engine* engine_instance;

    static void setEngine(Engine* engine);
    /**
     * Funkcja zamienia podaną wartość liczbową na tekst.
     * @param num - liczba całkowita
     * @return liczbę całkowitą jako tekst
     */
    static std::string intToString(int num);

    virtual Renderer* getRenderer() const = 0;
};

#endif // ENGINE_H
