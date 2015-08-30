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
        ew.generate_events.erase(
                std::remove_if(ew.generate_events.begin(), ew.generate_events.end(), [](GenerateEvent*e){delete e; return true;}),
                ew.generate_events.end());
        ew.player_id_events.erase(
                std::remove_if(ew.player_id_events.begin(), ew.player_id_events.end(), [](PlayerNameEvent*e){delete e; return true;}),
                ew.player_id_events.end());
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

void ReceivedEvents::addEvent(GenerateEvent *event, unsigned long pos, unsigned long event_count)
{
    printEvent(event, pos, event_count);
    if(event->frame_number.l_value <= current_frame) return;

    if(pos >= 0 && event_count > 0)
    {
        if(frame_events[event->frame_number.l_value].generate_events.empty())
        {
            frame_events[event->frame_number.l_value].generate_events.resize(event_count);
        }
        frame_events[event->frame_number.l_value].generate_events.at(pos) = event;
    }
    else
    {
        frame_events[event->frame_number.l_value].generate_events.push_back(event);
    }
}

void ReceivedEvents::addEvent(PlayerNameEvent *event, unsigned long pos, unsigned long event_count)
{
    printEvent(event, pos, event_count);
    if(event->frame_number.l_value <= current_frame) return;

    if(pos >= 0 && event_count > 0)
    {
        if(frame_events[event->frame_number.l_value].player_id_events.empty())
        {
            frame_events[event->frame_number.l_value].player_id_events.resize(event_count);
        }
        frame_events[event->frame_number.l_value].player_id_events.at(pos) = event;
    }
    else
    {
        frame_events[event->frame_number.l_value].player_id_events.push_back(event);
    }
}

void ReceivedEvents::printEvent(Event *event, unsigned long pos, unsigned long event_count)
{
//    int type = (int)(event->type);
    std::cout << "Type: " << event->type << " frame: " << event->frame_number.l_value << " pos: " << pos << " count: " << event_count << " current frame: " << current_frame << std::endl;
}


void TransmitEvents::addEvent(Event *event)
{
    events.push_back(event);
}
