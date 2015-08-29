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

    closesocket(sockets[0]);
    sockets.erase(sockets.begin());
}

void ClientTCP::sendData()
{
    //zdjęcie z kolejki odpowiednich eventów
    bool send_events = false;
    EnterCriticalSection(parent->critical_section);
        send_events = parent->shared_data->send_events;
    LeaveCriticalSection(parent->critical_section);

    int size = 15;
    char* buf = new char[size];

    buf[0] = 0;
    buf[1] = 3;

    buf[2] = 20;
    buf[3] = 20;
    buf[4] = 20;
    buf[5] = 20;

    buf[6] = 0;
    buf[7] = 20;
    buf[8] = 20;
    buf[9] = 20;

    buf[10] = 0;
    buf[11] = 0;
    buf[12] = 20;
    buf[13] = 20;

    buf[14] = 0;

    send(sockets[0], buf , size, 0);
    cout << "SEND " << size << endl;
}

void ClientTCP::readData()
{
    char buffer[30];
    int size = recv(sockets[0], buffer, sizeof(buffer), 0);
    if(size != SOCKET_ERROR && size >= 6)
    {
        addEventFromBuffer(buffer, size);
    }
}

