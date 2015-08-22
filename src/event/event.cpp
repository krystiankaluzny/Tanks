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

std::ostream& operator<<(std::ostream &out, Event &e)
{
    return out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
}

//================================================

CollisionEvent::CollisionEvent() :
    Event(COLLISION_EVENT, 14),
    collision_type(NONE)
{
}

void CollisionEvent::setByteArray(char *buffer)
{
    int index = 1;
    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    collision_type = static_cast<CollisionEvent::CollisionType>(buffer[index++]);

    id_object1.c_value[1] = buffer[index++];
    id_object1.c_value[2] = buffer[index++];
    id_object1.c_value[0] = buffer[index++];
    id_object1.c_value[3] = buffer[index++];

    id_object2.c_value[1] = buffer[index++];
    id_object2.c_value[2] = buffer[index++];
    id_object2.c_value[3] = buffer[index++];
    id_object2.c_value[0] = buffer[index++];
}

char *CollisionEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;

    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = collision_type;

    buffer[index++] = id_object1.c_value[0];
    buffer[index++] = id_object1.c_value[1];
    buffer[index++] = id_object1.c_value[2];
    buffer[index++] = id_object1.c_value[3];

    buffer[index++] = id_object2.c_value[0];
    buffer[index++] = id_object2.c_value[1];
    buffer[index++] = id_object2.c_value[2];
    buffer[index++] = id_object2.c_value[3];

    return buffer;
}

std::ostream& operator<<(std::ostream &out, CollisionEvent &e)
{
    out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
    return out << " CollisionType: " << e.collision_type << " ID1: " << e.id_object1.l_value << " ID2: " << e.id_object2.l_value;
}

//================================================

MoveEvent::MoveEvent() :
    Event(MOVE_EVENT, 10),
    move_direction(NONE)
{
}

void MoveEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    move_direction = static_cast<MoveEvent::Direction>(buffer[index++]);

    id_tank.c_value[0] = buffer[index++];
    id_tank.c_value[1] = buffer[index++];
    id_tank.c_value[2] = buffer[index++];
    id_tank.c_value[3] = buffer[index++];
}

char *MoveEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = move_direction = static_cast<MoveEvent::Direction>(buffer[index++]);

    buffer[index++] = id_tank.c_value[0];
    buffer[index++] = id_tank.c_value[1];
    buffer[index++] = id_tank.c_value[2];
    buffer[index++] = id_tank.c_value[3];
}

//================================================

FireEvent::FireEvent() : Event(FIRE_EVENT, 9)
{
}

void FireEvent::setByteArray(char *buffer)
{
    int index = 0;
    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    index++;//nie ma sepc type

    id_tank.c_value[0] = buffer[index++];
    id_tank.c_value[1] = buffer[index++];
    id_tank.c_value[2] = buffer[index++];
    id_tank.c_value[3] = buffer[index++];
}

char *FireEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;

    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = id_tank.c_value[0];
    buffer[index++] = id_tank.c_value[1];
    buffer[index++] = id_tank.c_value[2];
    buffer[index++] = id_tank.c_value[3];

    return buffer;
}

//================================================

GenerateEvent::GenerateEvent() : Event(GENERATE_EVENT, 10), object_type(NONE)
{
}

void GenerateEvent::setByteArray(char *buffer)
{
    int index = 0;
    //TODO uzupełnić
}

char *GenerateEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;

    return buffer;
}

//================================================

BonusEvent::BonusEvent() : Event(BONUS_EVENT, 10), bonus_type(NONE)
{
}

void BonusEvent::setByteArray(char *buffer)
{
    int index = 0;
    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    bonus_type = static_cast<BonusEvent::BonusEventType>(buffer[index++]);

    id_player.c_value[0] = buffer[index++];
    id_player.c_value[1] = buffer[index++];
    id_player.c_value[2] = buffer[index++];
    id_player.c_value[3] = buffer[index++];
}

char *BonusEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;

    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = bonus_type;

    buffer[index++] = id_player.c_value[0];
    buffer[index++] = id_player.c_value[1];
    buffer[index++] = id_player.c_value[2];
    buffer[index++] = id_player.c_value[3];

    return buffer;
}
