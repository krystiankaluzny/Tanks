#ifndef TCPCONNECTION
#define TCPCONNECTION

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include "../type.h"
#include "../event/event.h"
#include "../appthread.h"

class Network;

class TCPConnection
{
public:
    TCPConnection();
    void setParent(Network* parent);

    virtual bool init() = 0;
    virtual void run() = 0;
    virtual void close() = 0;

protected:
    Network* parent;

    const int wait_for_event_time = 100; //ms
    const u_short PORT = 3782;
};

#endif // TCPCONNECTION

