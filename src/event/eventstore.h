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
    std::vector<PlayerIdEvent*> player_id_events;
};

//get by client or server
class ReceivedEvents
{
public:
    ReceivedEvents();
    unsigned long current_frame;
    std::map<unsigned long, EventsWrapper> frame_events;

    void addEvent(GenerateEvent* event, unsigned long pos = 0, unsigned long event_count = 0);
    void addEvent(PlayerIdEvent* event, unsigned long pos = 0, unsigned long event_count = 0);
};

//send to server by client
class TransmitEvents
{
public:
    std::vector<Event*> events;
    void addEvent(Event* event);
};

#endif // EVENTSTORE_H
