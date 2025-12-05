#include "engine.h"

Engine* Engine::engine_instance = nullptr;

void Engine::setEngine(Engine* engine)
{
    Engine::engine_instance = engine;
}
