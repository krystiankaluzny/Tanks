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

private:
    void readData();
    void getLongData(LongData& event_index, LongData& events_count, char *buffer);

    vector<SOCKET> sockets;
    vector<WSAEVENT> sockets_event;
};

#endif // CLENTTCP_H
