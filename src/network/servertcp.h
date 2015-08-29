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
    void sendData();
    void readData();

private:

    //akceptowanie prośby od klienta o połączenie (dodanie nowego socketu)
    void acceptSocket();
    //zamknięcie socketu
    void closeSocket(int socket_index);
    //odczyt ramki z socketu
    void readSocket(int socket_index);

    void broadcast(char* buf, int size);
    void sendInit(SOCKET s);

    vector<SOCKET> sockets; //pierwszy socket jest socketem serwera
    vector<WSAEVENT> sockets_event;
};

#endif // SERVERTCP_H
