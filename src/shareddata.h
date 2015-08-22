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
     */
    bool send_events;

    /**
     * Numebr obecnej ramki gry.
     * Tylko wątek gry może zmieniać zmienną.
     */
    unsigned long current_frame_number;
};

#endif // SHAREDDATA_H
