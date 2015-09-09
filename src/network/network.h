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
    /**
     * Zmienna przechowująca czy wątek sieci działa, czy ma być zakończony.
     */
    bool is_running;
    /**
     * Aktualny stan sieci: klient, serwer lub wyłączony
     */
    NetworkState state;
    /**
     * Obiekt serwera gry.
     */
    ServerTCP server;
    /**
     * Obiekt klienta gry.
     */
    ClientTCP client;
};

#endif // NETWORK_H
