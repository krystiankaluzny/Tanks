#include "eventstore.h"


ReceivedEvents::ReceivedEvents()
{
    current_frame = 0;
}

void ReceivedEvents::addEvent(GenerateEvent *event, unsigned long pos, unsigned long event_count)
{
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

void ReceivedEvents::addEvent(PlayerIdEvent *event, unsigned long pos, unsigned long event_count)
{
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


void TransmitEvents::addEvent(Event *event)
{
    events.push_back(event);
}
