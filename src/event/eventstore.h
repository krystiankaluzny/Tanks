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
    std::vector<PlayerNameEvent*> player_id_events;
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

    void addEvent(GenerateEvent* event, unsigned long pos = 0, unsigned long event_count = 0);
    void addEvent(PlayerNameEvent* event, unsigned long pos = 0, unsigned long event_count = 0);

    void printEvent(Event* event, unsigned long pos = 0, unsigned long event_count = 0);
};

//send to server by client
class TransmitEvents
{
public:
    std::vector<Event*> events;
    void addEvent(Event* event);
};

#endif // EVENTSTORE_H
