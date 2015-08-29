#include "servertcp.h"
#include "network.h"

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
    setPlayerName(server_socket, "SERVER");
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
                cout << " Cos idzie " <<endl;
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
    EventsWrapper events;
    unsigned long current_frame = parent->getCurrentFrame();
    EnterCriticalSection(parent->critical_section);
        events = parent->shared_data->received_events.frame_events[current_frame + 3];
    LeaveCriticalSection(parent->critical_section);

//    std::cout <<"send farme " << current_frame << std::endl;

    char* buf;
    Event* e;
    LongData evet_index;
    LongData evets_count;

    auto frame_footer = [&](char* buf, int pos)
    {
        buf[pos] = evet_index.c_value[0];
        buf[pos + 1] = evet_index.c_value[1];
        buf[pos + 2] = evet_index.c_value[2];
        buf[pos + 3] = evet_index.c_value[3];

        buf[pos + 4] = evets_count.c_value[0];
        buf[pos + 5] = evets_count.c_value[1];
        buf[pos + 6] = evets_count.c_value[2];
        buf[pos + 7] = evets_count.c_value[3];
    };

    evets_count.l_value = events.generate_events.size();
    for(int i = 0; i < evets_count.l_value; ++i)
    {
        e = events.generate_events[i];
        evet_index.l_value = i;
        buf = e->getByteArray();

        frame_footer(buf, e->event_datagram_size);

        broadcast(buf, e->event_datagram_size);
        delete[] buf;
    }

    evets_count.l_value = events.player_id_events.size();
    for(int i = 0; i < evets_count.l_value; ++i)
    {
        std::cout <<"send " << i << std::endl;

        e = events.player_id_events[i];
        evet_index.l_value = i;
        buf = e->getByteArray();
        printHex(buf, e->event_datagram_size + 8);

        frame_footer(buf, e->event_datagram_size);

        broadcast(buf, e->event_datagram_size + 8);
        delete[] buf;
    }
}

void ServerTCP::readData()
{
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

    sendInit(client_socket);

    PlayerIdEvent *player = new PlayerIdEvent();
    player->frame_number.l_value = parent->getCurrentFrame() + 50;
    player->player_id.l_value = client_socket;
    player->name[0] = 'N';
    player->name[1] = 'o';
    player->name[2] = 'w';
    player->name[3] = 'y';

    std::cout <<"save " << player->frame_number.l_value << std::endl;

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->received_events.addEvent(player);
    LeaveCriticalSection(parent->critical_section);
//    setPlayerName(client_socket, "Nowy");

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

    removePlayerName(sockets[socket_index]);
    closesocket(sockets[socket_index]);
    sockets.erase(sockets.begin() + socket_index);
    cout << "Zamknieto socket" << endl;
}

void ServerTCP::readSocket(int socket_index)
{
    char buffer[30];
    int size = recv(sockets[socket_index], buffer, sizeof(buffer), 0); //odczytanie danych i zapis do bufora

    if(size != SOCKET_ERROR && size >= 6)
    {
        addEventFromBuffer(buffer, size);
    }
}

void ServerTCP::broadcast(char *buf, int size)
{
    for(int i = 1; i < sockets.size(); i++)
    {
        send(sockets[i], buf , size, 0);
    }
}

void ServerTCP::sendInit(SOCKET s)
{
    InitEvent init;
    init.current_frame.l_value = parent->getCurrentFrame();
    char* buf = init.getByteArray();

    std::cout << "SEND INIT " << init.bufferSize() << std::endl;
    send(s, buf , init.bufferSize(), 0);
}


