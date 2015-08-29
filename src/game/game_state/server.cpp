#include "server.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../../appconfig.h"
#include "menu.h"
#include "../game.h"

Server::Server(Game *parent) : GameState(parent)
{
    m_finished = false;
    m_get_names_time = 0;
    setNetworkState(NetworkState::SERVER);
}


void Server::draw()
{
    Renderer* renderer = Engine::getEngine().getRenderer();
    renderer->clear();

    renderer->drawRect(&AppConfig::map_rect, {0, 0, 0, 255}, true);
    renderer->drawRect(&AppConfig::status_rect, {0, 0, 0, 255}, true);

    //LOGO
    const SpriteData* logo = Engine::getEngine().getSpriteConfig()->getSpriteData(ST_TANKS_LOGO);
    SDL_Rect dst = {(AppConfig::map_rect.w + AppConfig::status_rect.w - logo->rect.w)/2, 20, logo->rect.w, logo->rect.h};
    renderer->drawObject(&logo->rect, &dst);

    // show content:
    int i = 0;
    SDL_Point p_dst;

    p_dst = {50, 100};
    renderer->drawText(&p_dst, std::string("No."), {255, 255, 220, 255}, 2);
    p_dst = {140, 100};
    renderer->drawText(&p_dst, std::string("ID"), {255, 255, 255, 255}, 2);
    p_dst = {270, 100};
    renderer->drawText(&p_dst, std::string("NAME"), {255, 255, 255, 255}, 2);
    dst = {25, 120, AppConfig::map_rect.w + AppConfig::status_rect.w - 50, 2};
    renderer->drawRect(&dst, {250, 250, 200, 255}, true);

    std::map<SOCKET,std::string>::iterator iter = m_player_name.begin();
    for (; iter != m_player_name.end(); ++iter)
    {
        p_dst = {55, 130 + i * 20};
        renderer->drawText(&p_dst, Engine::intToString(i + 1), {255, 255, 255, 255}, 2);
        p_dst = {140, 130 + i * 20};
        renderer->drawText(&p_dst, Engine::intToString(iter->first), {255, 255, 255, 255}, 2);
        p_dst = {270, 130 + i * 20};
        renderer->drawText(&p_dst, iter->second, {255, 255, 255, 255}, 2);
        i++;
    }

    renderer->flush();
}

void Server::update(Uint32 dt)
{
//    m_get_names_time += dt;
//    if(m_get_names_time > AppConfig::get_player_names_time)
//    {
//        getNames();
//        m_get_names_time = 0;
//    }
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

void Server::eventProcess(EventsWrapper &ev)
{
    SOCKET player_socket;
    std::string player_name;
    std::vector<PlayerIdEvent*> players = ev.player_id_events;

    for(PlayerIdEvent* p : players)
    {
        player_socket = p->player_id.l_value;
        m_player_name[player_socket] = std::string(p->name);
    }
}

bool Server::finished() const
{
    return m_finished;
}

GameState *Server::nextState()
{
    setNetworkState(NetworkState::NONE);
    return new Menu(parent);
}

void Server::getNames()
{
    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);
}
