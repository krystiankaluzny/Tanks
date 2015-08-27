#include "event.h"


Event::Event() :
    type(NONE_EVENT_TYPE),
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

KeyEvent::KeyEvent() :
    Event(KEY_EVENT_TYPE, 10)
    //TODOmove_direction(NONE)
{
}

void KeyEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    //TODO
//    move_direction = static_cast<MoveEvent::Direction>(buffer[index++]);

    id_tank.c_value[0] = buffer[index++];
    id_tank.c_value[1] = buffer[index++];
    id_tank.c_value[2] = buffer[index++];
    id_tank.c_value[3] = buffer[index++];
}

char *KeyEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    //TODO
    //buffer[index++] = move_direction = static_cast<MoveEvent::Direction>(buffer[index++]);

    buffer[index++] = id_tank.c_value[0];
    buffer[index++] = id_tank.c_value[1];
    buffer[index++] = id_tank.c_value[2];
    buffer[index++] = id_tank.c_value[3];
}

//================================================

GenerateEvent::GenerateEvent() : Event(GENERATE_EVENT_TYPE, 10)
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

