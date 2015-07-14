#include "servertcp.h"

ServerTCP::ServerTCP()
{
    is_running = false;
}

ServerTCP::~ServerTCP()
{

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

void ServerTCP::run()
{
    is_running = true;
    while(is_running)
    {
        if(sockets.size() > 0)
        {
            unsigned event_index = WSAWaitForMultipleEvents(sockets.size(), &sockets_event[0], false, 100, false); //czekaj na jakiś event
            if(event_index == WSA_WAIT_FAILED || event_index == WSA_WAIT_TIMEOUT)
                continue;
            else
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
    int size = recv(sockets[socket_index], buffer, sizeof(buffer), 0);

    if(size != SOCKET_ERROR && size >= 5)
    {
        int index = 0;
        char event_type = buffer[index++];
        if(event_type == COLLISION_EVENT)
        {
            CollisionEvent ce();
            if(ce.event_datagram_size == size)
            {
                ce.frame_number.c_value[0] = buffer[index++];
                ce.frame_number.c_value[1] = buffer[index++];
                ce.frame_number.c_value[2] = buffer[index++];
                ce.frame_number.c_value[3] = buffer[index++];

                ce.collision_type = static_cast<CollisionEvent::CollisionType>(buffer[index++]);

                ce.id_object1.c_value[0] = buffer[index++];
                ce.id_object1.c_value[1] = buffer[index++];
                ce.id_object1.c_value[2] = buffer[index++];
                ce.id_object1.c_value[3] = buffer[index++];

                ce.id_object2.c_value[0] = buffer[index++];
                ce.id_object2.c_value[1] = buffer[index++];
                ce.id_object2.c_value[2] = buffer[index++];
                ce.id_object2.c_value[3] = buffer[index++];

                events.insert(pair<SOCKET, Event>(sockets[socket_index], ce));
                cout << ce << endl;
            }
            else
                cout << "Zly rozmiar collision " << size << endl;
        }
        else if(event_type == MOVE_EVENT)
        {
            MoveEvent me();
            if(me.event_datagram_size == size)
            {
                me.frame_number.c_value[0] = buffer[index++];
                me.frame_number.c_value[1] = buffer[index++];
                me.frame_number.c_value[2] = buffer[index++];
                me.frame_number.c_value[3] = buffer[index++];

                me.move_direction = static_cast<MoveEvent::Direction>(buffer[index++]);

                me.id_tank.c_value[0] = buffer[index++];
                me.id_tank.c_value[1] = buffer[index++];
                me.id_tank.c_value[2] = buffer[index++];
                me.id_tank.c_value[3] = buffer[index++];

                events.insert( pair<SOCKET, Event>(sockets[socket_index], me));
            }
            else
                cout << "Zly rozmiar MoveEvent " << size << endl;
        }
        else if(event_type == FIRE_EVENT)
        {
            FireEvent fe();
            if(fe.event_datagram_size == size)
            {
                fe.frame_number.c_value[0] = buffer[index++];
                fe.frame_number.c_value[1] = buffer[index++];
                fe.frame_number.c_value[2] = buffer[index++];
                fe.frame_number.c_value[3] = buffer[index++];

                fe.id_tank.c_value[0] = buffer[index++];
                fe.id_tank.c_value[1] = buffer[index++];
                fe.id_tank.c_value[2] = buffer[index++];
                fe.id_tank.c_value[3] = buffer[index++];

            }
            else
                cout << "Zly rozmiar FireEvent " << size << endl;
        }
        else if(event_type == GENERATE_EVENT)
        {
            GenerateEvent ge();

        }
        else if(event_type == BONUS_EVENT)
        {
            BonusEvent be();
            if(be.event_datagram_size == size)
            {
                be.frame_number.c_value[0] = buffer[index++];
                be.frame_number.c_value[1] = buffer[index++];
                be.frame_number.c_value[2] = buffer[index++];
                be.frame_number.c_value[3] = buffer[index++];

                be.bonus_type = static_cast<BonusEvent::BonusEventType>(buffer[index++]);

                be.id_tank.c_value[0] = buffer[index++];
                be.id_tank.c_value[1] = buffer[index++];
                be.id_tank.c_value[2] = buffer[index++];
                be.id_tank.c_value[3] = buffer[index++];
            }
            else
                cout << "Zly rozmiar BonusEvent " << size << endl;
        }
    }
    else
        cout << "Zly rozmiar " << size << endl;
}
