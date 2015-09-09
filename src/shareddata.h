#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "event/eventstore.h"
#include "type.h"

#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include "myqueue.h"

class SharedData
{
public:
    SharedData();
    /**
     * Kolejka eventów do wysłania na server.
     */
    MyQueue<Event*> received_events_queue;

    /**
     * Tablica zdarzeń do wysłania
     */
    TransmitEvents transmit_events;
    /**
     * Status sieci: server, klient lub wyłączony
     */
    NetworkState network_state;
    /**
     * Zmienna przechowuje czy aplikacja działa. Służy do zamykania wątku sieci.
     * Tylko wątek gry może zmieniać zmienną.
     */
    bool run_app;

    //TODO czy to potrzebne
    /**
     * Jeśli zmienna jest równa true to klent TCP próbuje wysłać zebrane eventy.
     */
    bool send_events;

    /**
     * Numebr obecnej ramki gry.
     * Tylko wątek gry może zmieniać zmienną.
     */
    unsigned long current_frame_number;

    /**
     * Identyfikator gracza
     */
    unsigned long player_id;

    /**
     * Zbiór nazw graczy przypisanych socketom
     */
    std::map<SOCKET, std::string> player_name;

    /**
     * Nazwa lub ip serwera.
     */
    std::string host_name;

};

#endif // SHAREDDATA_H
