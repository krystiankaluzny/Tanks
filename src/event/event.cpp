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

int Event::bufferSize()
{
    return event_datagram_size + 8;
}

void Event::setZeroPos(char *buff, int starting_point)
{
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
    buff[starting_point++] = 0;
}

std::ostream& operator<<(std::ostream &out, Event &e)
{
    return out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
}

//================================================

KeyEvent::KeyEvent() :
    Event(KEY_EVENT_TYPE, 10),
    key_type(NONE)
{
}

void KeyEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];

    key_type = static_cast<KeyEvent::KeyType>(buffer[index++]);

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

    buffer[index++] = key_type;

    buffer[index++] = id_tank.c_value[0];
    buffer[index++] = id_tank.c_value[1];
    buffer[index++] = id_tank.c_value[2];
    buffer[index++] = id_tank.c_value[3];

    setZeroPos(buffer, event_datagram_size);

    return buffer;
}

//================================================

GenerateEvent::GenerateEvent() : Event(GENERATE_EVENT_TYPE, 17)
{
}

void GenerateEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];
    index++;    //no sepc type

    seed1.c_value[0] = buffer[index++];
    seed1.c_value[1] = buffer[index++];
    seed1.c_value[2] = buffer[index++];
    seed1.c_value[3] = buffer[index++];

    seed2.c_value[0] = buffer[index++];
    seed2.c_value[1] = buffer[index++];
    seed2.c_value[2] = buffer[index++];
    seed2.c_value[3] = buffer[index++];

    seed3.c_value[0] = buffer[index++];
    seed3.c_value[1] = buffer[index++];
    seed3.c_value[2] = buffer[index++];
    seed3.c_value[3] = buffer[index++];
}

char *GenerateEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = seed1.c_value[0];
    buffer[index++] = seed1.c_value[1];
    buffer[index++] = seed1.c_value[2];
    buffer[index++] = seed1.c_value[3];

    buffer[index++] = seed2.c_value[0];
    buffer[index++] = seed2.c_value[1];
    buffer[index++] = seed2.c_value[2];
    buffer[index++] = seed2.c_value[3];

    buffer[index++] = seed3.c_value[0];
    buffer[index++] = seed3.c_value[1];
    buffer[index++] = seed3.c_value[2];
    buffer[index++] = seed3.c_value[3];

    setZeroPos(buffer, event_datagram_size);

    return buffer;
}


PlayerNameEvent::PlayerNameEvent() : Event(PLAYER_ID_TYPE, 24)
{
}

void PlayerNameEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];
    index++;    //no sepc type

    player_id.c_value[0] = buffer[index++];
    player_id.c_value[1] = buffer[index++];
    player_id.c_value[2] = buffer[index++];
    player_id.c_value[3] = buffer[index++];

    for(int i = 0; i < 15; i++)
    {
        name[i] = buffer[index++];
    }
}

char *PlayerNameEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = player_id.c_value[0];
    buffer[index++] = player_id.c_value[1];
    buffer[index++] = player_id.c_value[2];
    buffer[index++] = player_id.c_value[3];

    for(int i = 0; i < 15; i++)
    {
        buffer[index++] = name[i];
    }

    setZeroPos(buffer, event_datagram_size);

    return buffer;
}


InitEvent::InitEvent() : Event(INIT_EVENT_TYPE, 13)
{

}

void InitEvent::setByteArray(char *buffer)
{
    int index = 1;

    frame_number.c_value[0] = buffer[index++];
    frame_number.c_value[1] = buffer[index++];
    frame_number.c_value[2] = buffer[index++];
    frame_number.c_value[3] = buffer[index++];
    index++;    //no sepc type

    current_frame.c_value[0] = buffer[index++];
    current_frame.c_value[1] = buffer[index++];
    current_frame.c_value[2] = buffer[index++];
    current_frame.c_value[3] = buffer[index++];

    player_id.c_value[0] = buffer[index++];
    player_id.c_value[1] = buffer[index++];
    player_id.c_value[2] = buffer[index++];
    player_id.c_value[3] = buffer[index++];

}

char *InitEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size + 8]; //+8 na event index i event count
    int index = 0;
    buffer[index++] = type;
    buffer[index++] = frame_number.c_value[0];
    buffer[index++] = frame_number.c_value[1];
    buffer[index++] = frame_number.c_value[2];
    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = current_frame.c_value[0];
    buffer[index++] = current_frame.c_value[1];
    buffer[index++] = current_frame.c_value[2];
    buffer[index++] = current_frame.c_value[3];

    buffer[index++] = player_id.c_value[0];
    buffer[index++] = player_id.c_value[1];
    buffer[index++] = player_id.c_value[2];
    buffer[index++] = player_id.c_value[3];

    setZeroPos(buffer, event_datagram_size);

    return buffer;
}
