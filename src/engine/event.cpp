#include "event.h"


Event::Event() :
    type(NONE_EVENT),
    event_datagram_size(0)
{

}

Event::Event(EventType type, int data_size) :
    type(type),
    event_datagram_size(data_size)
{

}

std::ostream &Event::operator<<(std::ostream &out, Event &e)
{
    return out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
}


CollisionEvent::CollisionEvent() :
    Event(COLLISION_EVENT, 14),
    collision_type(NONE)
{

}

std::ostream &CollisionEvent::operator<<(std::ostream &out, CollisionEvent &e)
{
    out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
    return out << " CollisionType: " << e.collision_type << " ID1: " << e.id_object1.l_value << " ID2: " << e.id_object2.l_value;
}


MoveEvent::MoveEvent() :
    Event(MOVE_EVENT, 10),
    move_direction(NONE)
{

}


FireEvent::FireEvent() : Event(FIRE_EVENT, 9)
{

}


GenerateEvent::GenerateEvent() : Event(GENERATE_EVENT, 10), object_type(NONE)
{

}

BonusEvent::BonusEvent() : Event(BONUS_EVENT, 10), bonus_type(NONE)
{

}
