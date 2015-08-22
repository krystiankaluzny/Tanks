#ifndef NETWORK_H
#define NETWORK_H

//#define _WIN32_WINNT 0x501

#include "../appthread.h"
#include "servertcp.h"
#include "clienttcp.h"

class Network : public AppThread
{
public:

    Network(SharedData* shared_data, CRITICAL_SECTION* critical_section);

    void run();

private:
    bool is_running;
    NetworkState state;
    ServerTCP server;
    ClientTCP client;
};

#endif // NETWORK_H
