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
    std::vector<GenerateEvent*> generate_events;
};

//send from server, get by client
class ReceivedEvents
{
public:
    ReceivedEvents();
    unsigned long current_frame;
    std::map<unsigned long, EventsWrapper> frame_events;

    void addEvent(GenerateEvent* event, unsigned long pos, unsigned long event_count);
};

//to send to server by client or get by server
class TransmitEvents
{
public:
    std::map<unsigned long, EventsWrapper> frame_events;
    void addEvent(Event* event);
};

#endif // EVENTSTORE_H
