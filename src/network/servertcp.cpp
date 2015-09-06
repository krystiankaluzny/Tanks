#include "servertcp.h"
#include "network.h"
#include "../appconfig.h"

ServerTCP::ServerTCP()
{
}

ServerTCP::~ServerTCP()
{
    close();
}

bool ServerTCP::init()
{
    //Inicjalizacja WinSock
    WSADATA ws_data; //dodatkowe dane biblioteki
    // używana wersja 2.2 winsock
    if(WSAStartup(MAKEWORD(2, 2), &ws_data) != NO_ERROR) return false;

    //utworzenie gniazda UDP
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    SOCKADDR_IN server_address;
    ZeroMemory(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT); //numer potu

    if(bind(server_socket, (PSOCKADDR)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        closesocket(server_socket);
        WSACleanup();
        return false;
    }

    if(listen(server_socket, 4) == SOCKET_ERROR)
    {
        closesocket(server_socket);
        WSACleanup();
        return false;
    }

    WSAEVENT server_event = WSACreateEvent();
    if(WSAEventSelect(server_socket, server_event, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
    {
        WSACloseEvent(server_event);
        closesocket(server_socket);
        WSACleanup();
        return false;
    }
    sockets.push_back(server_socket);
    sockets_event.push_back(server_event);
    setPlayerName(server_socket, "Player 1");

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->player_id = server_socket;
    LeaveCriticalSection(parent->critical_section);

    return true;
}

void ServerTCP::close()
{
    closeSocket(0);
}

void ServerTCP::run()
{
    if(sockets.size() > 0)
    {
        sendData();

        unsigned event_index = WSAWaitForMultipleEvents(sockets.size(), &sockets_event[0], false, wait_for_event_time, false); //czekaj na jakiś event
        if(event_index != WSA_WAIT_FAILED && event_index != WSA_WAIT_TIMEOUT)
        {
            WSANETWORKEVENTS network_events;
            if(WSAEnumNetworkEvents(sockets[event_index], sockets_event[event_index], &network_events) == SOCKET_ERROR)
            {

            }
            else if(network_events.lNetworkEvents & FD_ACCEPT)
            {
                acceptSocket();
            }
            else if(network_events.lNetworkEvents & FD_READ)
            {
                readSocket(event_index);
            }
            else if(network_events.lNetworkEvents & FD_CLOSE)
            {
                closeSocket(event_index);
                if(event_index == 0)
                {
                    EnterCriticalSection(parent->critical_section);
                        parent->shared_data->network_state = NetworkState::NONE;
                    LeaveCriticalSection(parent->critical_section);
                }
            }
        }
    }
}

void ServerTCP::sendData()
{
//    EnterCriticalSection(parent->critical_section);
//        events = parent->shared_data->received_events.frame_events[last_send_frame_events];
//    LeaveCriticalSection(parent->critical_section);

    char* buf;

//    int events_count = events.events.size();
//    for(int i = 0; i < events_count; ++i)
//    {
//        e = events.events[i];
//        buf = e->getByteArray();
////        std::cout <<"SERVER send " << i << " Frame to send " << current_frame + 10 << " frame event " << e->frame_number.l_value << std::endl;

//        if(e->type == EventType::POSITION_TYPE)
//        {
//            printHex(buf, e->bufferSize());
//        }
//        broadcast(buf, e->bufferSize());

//        delete[] buf;
//    }

    std::vector<Event*> transmit_events;
    EnterCriticalSection(parent->critical_section);
        transmit_events = parent->shared_data->transmit_events.events;
    LeaveCriticalSection(parent->critical_section);

    for(Event* e : transmit_events)
    {
        buf = e->getByteArray();
        broadcast(buf, e->bufferSize());

        EnterCriticalSection(parent->critical_section);
            parent->shared_data->received_events_queue.push(e);
        LeaveCriticalSection(parent->critical_section);

        delete[] buf;

        std::cout << "send_counter: " << (int)send_counter << std::endl;
    }
    //czyszczenie
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->transmit_events.events.clear();
    LeaveCriticalSection(parent->critical_section);
}

void ServerTCP::readData()
{
}


void ServerTCP::acceptSocket()
{
    if(sockets.size() < socket_limit)
    {
        SOCKADDR_IN client_address;
        int client_address_size = sizeof(client_address);
        SOCKET client_socket = accept(sockets[0], (struct sockaddr *)&client_address, &client_address_size); //zaakceptowanie prośby o połączenie
        if(client_socket == SOCKET_ERROR) return;

        WSAEVENT client_event = WSACreateEvent();
        if(WSAEventSelect(client_socket, client_event, FD_READ | FD_CLOSE) == INVALID_SOCKET)
        {
            WSACloseEvent(client_event);
            return;
        }
        sockets.push_back(client_socket);
        sockets_event.push_back(client_event);

        sendInit(client_socket);

        cout << "Otwarto socket" << endl;
    }
}

void ServerTCP::closeSocket(int socket_index)
{
    if(socket_index >= sockets_event.size() || socket_index < 0) return;
    if(socket_index == 0)
    {
        while(sockets.size() > 1) closeSocket(1);

        clearNames();
    }
    WSACloseEvent(sockets_event[socket_index]);
    sockets_event.erase(sockets_event.begin() + socket_index);

    //removePlayerName(sockets[socket_index]);    //usuwanie gracza z shared data player_name


    DisconnectEvent* event = new DisconnectEvent;
    event->player_id.l_value = sockets[socket_index];

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->received_events_queue.push(event);
    LeaveCriticalSection(parent->critical_section);

    closesocket(sockets[socket_index]);
    sockets.erase(sockets.begin() + socket_index);
    cout << "Zamknieto socket" << endl;
}

void ServerTCP::readSocket(int socket_index)
{
    char buffer[50];
    int size = recv(sockets[socket_index], buffer, sizeof(buffer), 0); //odczytanie danych i zapis do bufora

    if(size != SOCKET_ERROR && size >= 6)
    {
//        cout << " Cos idzie z " << sockets[socket_index] << endl;
        addEventFromBuffer(buffer, size);
    }
}

void ServerTCP::broadcast(char *buf, int size)
{
    for(int i = 1; i < sockets.size(); i++)
    {

        cout << " Type send " << (int)buf[0] << endl;
        send(sockets[i], buf , size, 0);
        send_counter++;
    }
}

void ServerTCP::sendInit(SOCKET s)
{
    InitEvent init;
    init.current_frame.l_value = parent->getCurrentFrame();
    init.player_id.l_value = s;

    char* buf = init.getByteArray();

    send(s, buf , init.bufferSize(), 0);
    delete[] buf;
}


