#include "engine.h"


Engine::Engine()
{
    m_renderer = nullptr;
    m_sprite_config = nullptr;
}

Engine &Engine::getEngine()
{
    static Engine engine;
    return engine;
}

void Engine::initModules()
{
    m_renderer = new Renderer;
    m_sprite_config = new SpriteConfig;
}

void Engine::destroyModules()
{
    delete m_renderer;
    m_renderer = nullptr;
    delete m_sprite_config;
    m_sprite_config = nullptr;
}

Renderer *Engine::getRenderer() const
{
    return m_renderer;
}

SpriteConfig *Engine::getSpriteConfig() const
{
    return m_sprite_config;
}
