#ifndef NETWORK_H
#define NETWORK_H

#include "../appthread.h"
#include "servertcp.h"
#include "clenttcp.h"

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
