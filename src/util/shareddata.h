#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "../event/eventstore.h"
#include "../type.h"
#include "myqueue.h"
#include "../game/objects/object.h"

#include <map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
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

    unsigned long player_id;

    std::map<SOCKET, std::string> player_name;

    MyQueue<SDL_Event> sdl_events_queue;

    MyQueue<Object*> object_to_render;
    /**
     * Usuwanie eventów z wybranej ramki.
     * @param frame -1 oznacza usunięcie wszystkiego
     */
    void clearReceiveEvents(unsigned long frame);

    /**
     * Usuwanie eventów z obecnej ramki
     */
    void clearCurrentEvents();
    /**
     * Ustawienie obecnej ramki gry w SharedData oraz w ReceivedEvents.
     * @param current_frame
     */
    void setCurrentFrameNumber(unsigned long current_frame);

    void incrementFrameNumber();
};

#endif // SHAREDDATA_H
