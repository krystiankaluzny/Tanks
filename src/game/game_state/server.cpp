#include "server.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../../appconfig.h"

Server::Server(Game *parent) : GameState(parent)
{
    m_finished = false;
}


void Server::draw()
{
    Renderer* renderer = Engine::getEngine().getRenderer();
    renderer->clear();

    renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer->drawRect(&AppConfig::status_rect, {0, 0, 0, 255}, true);
}

void Server::update(Uint32 dt)
{

}

void Server::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_ESCAPE)
        {
            m_finished = true;
        }
    }
}

bool Server::finished() const
{
    return m_finished;
}

GameState *Server::nextState()
{
    return nullptr;
}
