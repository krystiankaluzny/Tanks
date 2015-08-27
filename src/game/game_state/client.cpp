#include "client.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../../appconfig.h"
#include "menu.h"
#include "../game.h"

Client::Client(Game *parent) : GameState(parent)
{
    m_finished = false;
}

bool Client::finished() const
{
    return m_finished;
}

void Client::draw()
{
    Renderer* renderer = Engine::getEngine().getRenderer();
    renderer->clear();

    renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer->drawRect(&AppConfig::status_rect, {0, 0, 0, 255}, true);

    //LOGO
    const SpriteData* logo = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_TANKS_LOGO);
    SDL_Rect dst = {(AppConfig::map_rect.w + AppConfig::status_rect.w - logo->rect.w)/2, 20, logo->rect.w, logo->rect.h};
    renderer->drawObject(&logo->rect, &dst);

    renderer->flush();
}

void Client::update(Uint32 dt)
{

}

void Client::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_ESCAPE)
        {
            m_finished = true;
        }
    }
}

GameState *Client::nextState()
{
    return new Menu(parent);
}

