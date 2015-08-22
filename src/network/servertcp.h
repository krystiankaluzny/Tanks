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

#include "../event/event.h"
#include "../appthread.h"
#include "tcpconnection.h"

using namespace std;

class ServerTCP : public TCPConnection
{
public:
    ServerTCP();
    ~ServerTCP();

    void run();
    bool init();
    void close();
private:

    void mainLoop();

    //akceptowanie prośby od klienta o połączenie (dodanie nowego socketu)
    void acceptSocket();
    //zamknięcie socketu
    void closeSocket(int socket_index);
    //odczyt ramki z socketu
    void readSocket(int socket_index);

    //jeśli wszystko ok dodaje event do kolejki
    void addEvent(Event* ev, char* data, int size, int socket_index);

    const int wait_for_event_time = 100; //ms

    const u_short PORT = 3782;
    multimap<SOCKET, Event*> events;
    vector<SOCKET> sockets; //pierwszy socket jest soketem serwera
    vector<WSAEVENT> sockets_event;
};

#endif // SERVERTCP_H
