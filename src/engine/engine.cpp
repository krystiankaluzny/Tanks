#include "engine.h"


Engine::Engine()
{
    m_renderer = nullptr;
    m_sprite_data = nullptr;
}

Engine &Engine::getEngine()
{
    static Engine engine;
    return engine;
}

void Engine::initModules()
{
    m_renderer = new Renderer;
    m_sprite_data = new SpriteConfig;
}

void Engine::destroyModules()
{
    delete m_renderer;
    m_renderer = nullptr;
    delete m_sprite_data;
    m_sprite_data = nullptr;
}

Renderer *Engine::getRenderer() const
{
    return m_renderer;
}

SpriteConfig *Engine::getSpriteConfig() const
{
    return m_sprite_data;
}
