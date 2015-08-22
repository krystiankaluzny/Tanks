#ifndef SERVERTCP_H
#define SERVERTCP_H

//#define _WIN32_WINNT 0x501

#include <iostream>
#include <vector>
#include <map>

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

    //akceptowanie prośby od klienta o połączenie (dodanie nowego socketu)
    void acceptSocket();
    //zamknięcie socketu
    void closeSocket(int socket_index);
    //odczyt ramki z socketu
    void readSocket(int socket_index);

    //jeśli wszystko ok dodaje event do kolejki
    void addEvent(Event* ev, char* data, int size, int socket_index);

    vector<SOCKET> sockets; //pierwszy socket jest soketem serwera
    vector<WSAEVENT> sockets_event;
};

#endif // SERVERTCP_H
