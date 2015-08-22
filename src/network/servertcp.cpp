#include "servertcp.h"

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
                cout << " Cos idzie " <<endl;
                readSocket(event_index);
            }
            else if(network_events.lNetworkEvents & FD_CLOSE)
            {
                closeSocket(event_index);
            }
        }
    }
}

void ServerTCP::acceptSocket()
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
    cout << "Otwarto socket" << endl;
}

void ServerTCP::closeSocket(int socket_index)
{
    if(socket_index >= sockets_event.size() || socket_index < 0) return;
    if(socket_index == 0)
    {
        while(sockets.size() > 1) closeSocket(1);
    }
    WSACloseEvent(sockets_event[socket_index]);
    sockets_event.erase(sockets_event.begin() + socket_index);

    closesocket(sockets[socket_index]);
    sockets.erase(sockets.begin() + socket_index);
    cout << "Zamknieto socket" << endl;
}

void ServerTCP::readSocket(int socket_index)
{
    char buffer[20];
    int size = recv(sockets[socket_index], buffer, sizeof(buffer), 0); //odczytanie danych i zapis do bufora

    if(size != SOCKET_ERROR && size >= 5)
    {
        int index = 0;
        char event_type = buffer[index++];
        Event* event = nullptr;
        switch(event_type)
        {
        case COLLISION_EVENT:
            event = new CollisionEvent;
            break;
        case MOVE_EVENT:
            event = new MoveEvent;
            break;
        case FIRE_EVENT:
            event = new FireEvent;
            break;
        case GENERATE_EVENT:
            event = new GenerateEvent;
            break;
        case BONUS_EVENT:
            event = new BonusEvent;
            break;
        }
        addEvent(event, buffer, size, socket_index);
    }
}

void ServerTCP::addEvent(Event *ev, char *data, int size, int socket_index)
{
    if(ev == nullptr) return;
    if(ev->event_datagram_size == size)
    {
        ev->setByteArray(data);
        events.insert( pair<SOCKET, Event*>(sockets[socket_index], ev));
    }
    else
    {
        cout << "Zly rozmiar eventu " << size << endl;
    }
}

