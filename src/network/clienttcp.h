#ifndef CLENTTCP_H
#define CLENTTCP_H

#include "tcpconnection.h"
#include "vector"

using namespace std;

class ClientTCP : public TCPConnection
{
public:
    ClientTCP();
    ~ClientTCP();

    bool init();
    void run();
    void close();
    void sendData();
    void readData();
    string hostName;
};

#endif // CLENTTCP_H
