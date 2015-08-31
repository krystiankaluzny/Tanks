#ifndef EVENTSTORE_H
#define EVENTSTORE_H

#include "event.h"
#include <vector>
#include <map>

class EventsWrapper
{
public:
    EventsWrapper() {}
    unsigned long events_count;
    std::vector<Event*> events;
};

//get by client or server
class ReceivedEvents
{
public:
    ReceivedEvents();
    unsigned long current_frame;
    std::map<unsigned long, EventsWrapper> frame_events;

    /**
     * @param frame jeśli -1 usuń wszystko
     */
    void clearFrameEvents(unsigned long frame);

    void addEvent(Event* event);

    void printEvent(Event* event);
};

//send to server by client
class TransmitEvents
{
public:
    std::vector<Event*> events;
    void addEvent(Event* event);
};

#endif // EVENTSTORE_H
