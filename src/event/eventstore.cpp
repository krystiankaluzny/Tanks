#include "eventstore.h"


ReceivedEvents::ReceivedEvents()
{
    current_frame = 0;
}

void ReceivedEvents::addEvent(CollisionEvent *event, unsigned long pos, unsigned long event_count)
{
    if(event->frame_number.l_value > current_frame)
    {
        if(frame_events.at(event->frame_number.l_value).collision_events.empty())
        {
            frame_events.at(event->frame_number.l_value).collision_events.reserve(event_count);
        }
        frame_events.at(event->frame_number.l_value).collision_events.at(pos) = event;
    }
}

void ReceivedEvents::addEvent(MoveEvent *event, unsigned long pos, unsigned long event_count)
{
    if(event->frame_number.l_value > current_frame)
    {
        if(frame_events.at(event->frame_number.l_value).move_events.empty())
        {
            frame_events.at(event->frame_number.l_value).move_events.reserve(event_count);
        }
        frame_events.at(event->frame_number.l_value).move_events.at(pos) = event;
    }
}

void ReceivedEvents::addEvent(FireEvent *event, unsigned long pos, unsigned long event_count)
{
    if(event->frame_number.l_value > current_frame)
    {
        if(frame_events.at(event->frame_number.l_value).fire_events.empty())
        {
            frame_events.at(event->frame_number.l_value).fire_events.reserve(event_count);
        }
        frame_events.at(event->frame_number.l_value).fire_events.at(pos) = event;
    }
}

void ReceivedEvents::addEvent(GenerateEvent *event, unsigned long pos, unsigned long event_count)
{
    if(event->frame_number.l_value > current_frame)
    {
        if(frame_events.at(event->frame_number.l_value).generate_events.empty())
        {
            frame_events.at(event->frame_number.l_value).generate_events.reserve(event_count);
        }
        frame_events.at(event->frame_number.l_value).generate_events.at(pos) = event;
    }
}

void ReceivedEvents::addEvent(BonusEvent *event, unsigned long pos, unsigned long event_count)
{
    if(event->frame_number.l_value > current_frame)
    {
        if(frame_events.at(event->frame_number.l_value).bonus_events.empty())
        {
            frame_events.at(event->frame_number.l_value).bonus_events.reserve(event_count);
        }
        frame_events.at(event->frame_number.l_value).bonus_events.at(pos) = event;
    }
}

void TransmitEvents::addEvent(Event *event)
{
    frame_events.at(event->frame_number.l_value).events.push_back(event);
}
