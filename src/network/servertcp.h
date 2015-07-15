#ifndef SERVERTCP_H
#define SERVERTCP_H

//#define _WIN32_WINNT 0x501

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

#include "event.h"

using namespace std;

class ServerTCP
{
public:
    ServerTCP();
    ~ServerTCP();

    bool init();
    void run();

private:
    //akceptowanie prośby od klienta o połączenie (dodanie nowego socketu)
    void acceptSocket();
    //zamknięcie socketu
    void closeSocket(int socket_index);
    //odczyt ramki z socketu
    void readSocket(int socket_index);

    bool is_running;
    const u_short PORT = 3782;
    multimap<SOCKET, Event> events;
    vector<SOCKET> sockets; //pierwszy socket jest soketem serwera
    vector<WSAEVENT> sockets_event;
};

#endif // SERVERTCP_H
