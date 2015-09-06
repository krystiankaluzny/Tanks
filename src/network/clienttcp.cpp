#include "clienttcp.h"
#include "network.h"

ClientTCP::ClientTCP()
{

}

ClientTCP::~ClientTCP()
{
    close();
}

bool ClientTCP::init()
{
    //Inicjalizacja WinSock
    WSADATA ws_data; //dodatkowe dane biblioteki
    // używana wersja 2.2 winsock
    if(WSAStartup(MAKEWORD(2, 2), &ws_data) != NO_ERROR) return false;

    //utworzenie gniazda TCP
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_socket == SOCKET_ERROR)
    {
        WSACleanup();
        return false;
    }

    PHOSTENT host;
    if((host = gethostbyname("tank"))==NULL)
    {
        closesocket(client_socket);
        WSACleanup();
        return false;
    }

    SOCKADDR_IN server_address;
    ZeroMemory(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = *((u_long*)host->h_addr); //adres IP4 servera
    server_address.sin_port = htons(PORT); //numer potu

    char *hostname = new char[128];
    gethostname(hostname,128);

    cout << inet_ntoa(server_address.sin_addr) << endl;

    if(connect(client_socket, (PSOCKADDR)&server_address, sizeof(server_address)) == SOCKET_ERROR)
    {
        cout << "Nie mozna polaczyc" << endl;
        closesocket(client_socket);
        WSACleanup();
        return false;
    }

    WSAEVENT client_event = WSACreateEvent();
    if(WSAEventSelect(client_socket, client_event, FD_READ | FD_CLOSE) == SOCKET_ERROR)
    {
        WSACloseEvent(client_event);
        closesocket(client_socket);
        WSACleanup();
        return false;
    }
    sockets.push_back(client_socket);
    sockets_event.push_back(client_event);
    return true;
}

void ClientTCP::run()
{
    sendData();

    unsigned event_index = WSAWaitForMultipleEvents(sockets.size(), &sockets_event[0], false, wait_for_event_time, false);
    if(event_index != WSA_WAIT_FAILED && event_index != WSA_WAIT_TIMEOUT)
    {
        WSANETWORKEVENTS network_events;
        if(WSAEnumNetworkEvents(sockets[event_index], sockets_event[event_index], &network_events) == SOCKET_ERROR);
        else if(network_events.lNetworkEvents & FD_READ)
        {
            readData();
        }
        else if(network_events.lNetworkEvents & FD_CLOSE)
        {
            close();
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->network_state = NetworkState::NONE;
            LeaveCriticalSection(parent->critical_section);
        }
    }
}

void ClientTCP::close()
{
    if(sockets_event.size() == 0) return;
    WSACloseEvent(sockets_event[0]);
    sockets_event.erase(sockets_event.begin());

    clearNames();   //usuwanie wszystkich graczy z listy w shared data, żeby przy kolejnej próbie łączenia lista była pusta

    closesocket(sockets[0]);
    sockets.erase(sockets.begin());
}

void ClientTCP::sendData()
{
    //zdjęcie z kolejki odpowiednich eventów
    std::vector<Event*> transmit_events;
    long current = parent->getCurrentFrame();
    EnterCriticalSection(parent->critical_section);
//        send_events = parent->shared_data->send_events;
        transmit_events = parent->shared_data->transmit_events.events;
    LeaveCriticalSection(parent->critical_section);

    char* buf;

    for(Event* e : transmit_events)
    {
        buf = e->getByteArray();
        send(sockets[0], buf , e->bufferSize(), 0);
        send_counter++;
        std::cout << "send_counter: " << (int)send_counter << std::endl;

        EnterCriticalSection(parent->critical_section);
            parent->shared_data->received_events_queue.push(e);
        LeaveCriticalSection(parent->critical_section);

        delete[] buf;
    }

    //czyszczenie
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->transmit_events.events.clear();
    LeaveCriticalSection(parent->critical_section);
}

void ClientTCP::readData()
{
    int size = recv(sockets[0], buffer, sizeof(buffer), 0);
    if(size != SOCKET_ERROR && size >= 2)
    {
        addEventFromBuffer(buffer, size);
    }
}


