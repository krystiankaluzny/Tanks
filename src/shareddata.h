#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "event/eventstore.h"
#include "type.h"

class SharedData
{
public:
    SharedData();
    /**
     * Kolejka eventów do wysłania na server.
     */
    TransmitEvents transmit_events;
    /**
     * Eventy odebrane z servera
     */
    ReceivedEvents received_events;
    /**
     * Status sieci: server, klient lub wyłączony
     */
    NetworkState network_state;
    /**
     * Zmienna przechowuje czy aplikacja działa. Służy do zamykania wątku sieci.
     * Tylko wątek gry może zmieniać zmienną.
     */
    bool run_app;

    /**
     * Jeśli zmienna jest równa true to klent TCP próbuje wysłać zebrane eventy.
     * Tylko wątek gry może zmieniać zmienną.
     */
    bool send_events;
};

#endif // SHAREDDATA_H
