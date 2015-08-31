#include "eventstore.h"
#include <algorithm>

ReceivedEvents::ReceivedEvents()
{
    current_frame = 0;
}

void ReceivedEvents::clearFrameEvents(unsigned long frame)
{
    auto removeWrapper = [&](EventsWrapper& ew)
    {
        ew.events.erase(
                std::remove_if(ew.events.begin(), ew.events.end(), [](Event*e){delete e; return true;}),
                ew.events.end());
    };

    if(frame > 0)
    {
        EventsWrapper wrapper = frame_events[frame];

        removeWrapper(wrapper);

        frame_events.erase(frame);
    }
    else
    {
        for(auto& kv : frame_events)
        {
            EventsWrapper wrapper = kv.second;
            removeWrapper(wrapper);
        }
        frame_events.clear();
    }
}

void ReceivedEvents::addEvent(Event *event)
{
    printEvent(event);
    if(event->frame_number.l_value <= current_frame) return;

    frame_events[event->frame_number.l_value].events.push_back(event);
}


void ReceivedEvents::printEvent(Event *event)
{
//    int type = (int)(event->type);
    std::cout << "Type: " << event->type << " frame: " << event->frame_number.l_value << std::endl;
}


void TransmitEvents::addEvent(Event *event)
{
    events.push_back(event);
}
