#include "server.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../../appconfig.h"
#include "menu.h"
#include "../game.h"
#include "../game_state/networkbattle.h"

#include <cstdio>

Server::Server(Game *parent) : GameState(parent)
{
    m_finished = false;
    m_get_names_time = 0;
    m_send_names_time = 0;
    setNetworkState(NetworkState::SERVER);

    m_menu_texts.push_back("Start Game");
    m_menu_texts.push_back("Exit");
    m_menu_index = 0;

    m_tank_pointer = new Player(0, 0 , ST_PLAYER_1);
    m_tank_pointer->direction = D_RIGHT;
    m_tank_pointer->pos_x = 144;
    m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 290;
    m_tank_pointer->setFlag(TSF_LIFE);
    m_tank_pointer->update(0);
    m_tank_pointer->clearFlag(TSF_LIFE);
    m_tank_pointer->clearFlag(TSF_SHIELD);
    m_tank_pointer->setFlag(TSF_MENU);

    m_start_game = false;
}

Server::~Server()
{
    delete m_tank_pointer;
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

    //menu
    i = 0;
    SDL_Point text_start;
    for(auto text : m_menu_texts)
    {
        text_start = { 180, (i + 1) * 32 + 300};
        i++;
        renderer->drawText(&text_start, text, {255, 255, 255, 255}, 2);
    }

    m_tank_pointer->draw();

    renderer->flush();
}

void Server::update(Uint32 dt)
{
    m_get_names_time += dt;
    if(m_get_names_time > AppConfig::get_players_names_time)
    {
        getNames();
        m_get_names_time = 0;
    }

    m_send_names_time += dt;
    if(m_send_names_time > AppConfig::send_players_names_time)
    {
        sendNames();
        m_send_names_time = 0;
    }

    m_tank_pointer->speed = m_tank_pointer->default_speed;
    m_tank_pointer->stop = true;
    m_tank_pointer->update(dt);
}

void Server::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        if(ev->key.keysym.sym == SDLK_UP)
        {
            m_menu_index--;
            if(m_menu_index < 0)
                m_menu_index = m_menu_texts.size() - 1;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 290;
        }
        else if(ev->key.keysym.sym == SDLK_DOWN)
        {
            m_menu_index++;
            if(m_menu_index >= m_menu_texts.size())
                m_menu_index = 0;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 290;
        }
        else if(ev->key.keysym.sym == SDLK_SPACE || ev->key.keysym.sym == SDLK_RETURN)
        {
            if(m_menu_index == 0)
            {
                StartGameEvent* start_game = new StartGameEvent;
                start_game->frame_number.l_value = parent->getCurrentFrame() + start_game->priority;
                std::cout << "Start game" << std::endl;
                EnterCriticalSection(parent->critical_section);
                    parent->shared_data->newEvent(start_game);
//                    parent->shared_data->transmit_events.addEvent(start_game);
                LeaveCriticalSection(parent->critical_section);
                std::cout << "Start game2" << std::endl;
            }
            else
            {
                m_finished = true;
            }
        }
        else if(ev->key.keysym.sym == SDLK_ESCAPE)
        {
            m_menu_index = m_menu_texts.size() - 1;
            m_finished = true;
        }
    }
}

void Server::eventProcess(EventsWrapper &ev)
{
    SOCKET player_socket;
    std::vector<Event*> events = ev.events;

    if(events.size() == 0) return;

    for(Event* e : events)
    {
        switch (e->type)
        {
        case EventType::PLAYER_ID_TYPE:
        {
            PlayerNameEvent* p = (PlayerNameEvent*)e;
            player_socket = p->player_id.l_value;
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->player_name[player_socket] = std::string(p->name);
            LeaveCriticalSection(parent->critical_section);
            break;
        }
        case EventType::DISCONNECT_EVENT_TYPE:
        {
            DisconnectEvent* d = (DisconnectEvent*)e;
            player_socket = d->player_id.l_value;
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->player_name.erase(player_socket);
            LeaveCriticalSection(parent->critical_section);
            break;
        }
        case EventType::START_GAME_TYPE:
        {
            m_start_game = true;
            m_finished = true;

            break;
        }
        default:
            break;
        }
    }
}

bool Server::finished() const
{
    return m_finished;
}

GameState *Server::nextState()
{
    if(m_menu_index == m_menu_texts.size() - 1)
    {
        setNetworkState(NetworkState::NONE);
        return new Menu(parent);
    }
    else if(m_start_game)
    {
        std::cout << "Start game4" << std::endl;
        return new NetworkBattle(parent);
    }
    return nullptr;
}

void Server::getNames()
{
    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);
}

void Server::sendNames()
{
    for(auto player : m_player_name)
    {
        PlayerNameEvent *player_event = new PlayerNameEvent();
        player_event->frame_number.l_value = parent->getCurrentFrame() + player_event->priority;
        player_event->player_id.l_value = player.first;

        sprintf(player_event->name, "%s", player.second.c_str());

        EnterCriticalSection(parent->critical_section);
            parent->shared_data->newEvent(player_event);
        LeaveCriticalSection(parent->critical_section);
    }
}
