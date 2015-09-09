#include "client.h"
#include "../engine/engine.h"
#include "../engine/renderer.h"
#include "../../appconfig.h"
#include "menu.h"
#include "../game.h"
#include "networkbattle.h"

#include <cstdio>

Client::Client(Game *parent) : GameState(parent)
{
    m_finished = false;

    m_get_names_time = 0;
    m_send_name_time = 0;

    host = "localhost";

    m_menu_texts.push_back("Host: ");
    m_menu_texts.push_back("Join");
    m_menu_texts.push_back("Exit");
    m_menu_index = 0;

    m_tank_pointer = new Player(0, 0 , ST_PLAYER_1);
    m_tank_pointer->direction = D_RIGHT;
    m_tank_pointer->pos_x = 80;
    m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 270;
    m_tank_pointer->setFlag(TSF_LIFE);
    m_tank_pointer->update(0);
    m_tank_pointer->clearFlag(TSF_LIFE);
    m_tank_pointer->clearFlag(TSF_SHIELD);
    m_tank_pointer->setFlag(TSF_MENU);

    m_start_game = false;
}

Client::~Client()
{
    delete m_tank_pointer;
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
        text_start = { 125, (i + 1) * 32 + 280};
        if(i == 0)
        {
            text += host;
        }

        i++;
        renderer->drawText(&text_start, text, {255, 255, 255, 255}, 2);
    }

    m_tank_pointer->draw();

    renderer->flush();
}

void Client::update(Uint32 dt)
{
    m_get_names_time += dt;
    if(m_get_names_time > AppConfig::get_players_names_time)
    {
        getNames();
        m_get_names_time = 0;
    }

    NetworkState state;
    EnterCriticalSection(parent->critical_section);
        state = parent->shared_data->network_state;
    LeaveCriticalSection(parent->critical_section);

    if(state == NetworkState::CLIENT_INITIALIZED)
    {
        m_send_name_time += dt;
        if(m_send_name_time > AppConfig::send_players_names_time)
        {
            sendName();
            m_send_name_time = 0;
        }
    }

    m_tank_pointer->speed = m_tank_pointer->default_speed;
    m_tank_pointer->stop = true;
    m_tank_pointer->update(dt);
}

void Client::eventProcess(SDL_Event *ev)
{
    if(ev->type == SDL_KEYDOWN)
    {
        SDL_Keycode key = ev->key.keysym.sym;
        if(key == SDLK_ESCAPE)
        {
            m_finished = true;
        }
        else if(key == SDLK_UP)
        {
            m_menu_index--;
            if(m_menu_index < 0)
                m_menu_index = m_menu_texts.size() - 1;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 270;
        }
        else if(key == SDLK_DOWN)
        {
            m_menu_index++;
            if(m_menu_index >= m_menu_texts.size())
                m_menu_index = 0;

            m_tank_pointer->pos_y = (m_menu_index + 1) * 32 + 270;
        }
        else if(key == SDLK_SPACE || key == SDLK_RETURN)
        {
            if(m_menu_index == 1)
            {
                EnterCriticalSection(parent->critical_section);
                    parent->shared_data->host_name = host;
                LeaveCriticalSection(parent->critical_section);
                setNetworkState(NetworkState::CLIENT);
            }
            else if(m_menu_index == 2)
            {
                m_finished = true;
            }
        }
        if(m_menu_index == 0)
        {
            if(key == SDLK_BACKSPACE)
            {
                host = host.substr(0,host.length()-1);
            }
            else if(key == SDLK_DELETE)
            {
                host = "";
            }
            else if(key >= SDLK_a && key <= SDLK_z || key == SDLK_PERIOD || key >= SDLK_0 && key <= SDLK_9)
            {
                host += (char)(key);
            }
        }
    }
}

void Client::eventProcess()
{
    SOCKET player_socket;
    bool is_empty = true;
    Event* e = nullptr;
    EnterCriticalSection(parent->critical_section);
        is_empty = parent->shared_data->received_events_queue.empty();
    LeaveCriticalSection(parent->critical_section);
    while(!is_empty)
    {
        EnterCriticalSection(parent->critical_section);
            e = parent->shared_data->received_events_queue.pop();
        LeaveCriticalSection(parent->critical_section);
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

        EnterCriticalSection(parent->critical_section);
            is_empty = parent->shared_data->received_events_queue.empty();
        LeaveCriticalSection(parent->critical_section);
    }
}

GameState *Client::nextState()
{
    if(m_start_game)
    {
        return new NetworkBattle(parent, 2);
    }
    setNetworkState(NetworkState::NONE);
    return new Menu(parent);
}

void Client::getNames()
{
    EnterCriticalSection(parent->critical_section);
        m_player_name = parent->shared_data->player_name;
    LeaveCriticalSection(parent->critical_section);
}

void Client::sendName()
{
    PlayerNameEvent *player = new PlayerNameEvent();
    player->player_id.l_value = parent->getPlayerId();
    sprintf(player->name, "Player 2");

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->transmit_events.addEvent(player);
    LeaveCriticalSection(parent->critical_section);
}


