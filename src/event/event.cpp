#include "event.h"
#include "../appconfig.h"

Event::Event() :
    type(NONE_EVENT_TYPE),
    event_datagram_size(0),
    priority(AppConfig::sending_events_offset + 5)
{
}

Event::Event(EventType type, int data_size, int priority) :
    type(type),
    event_datagram_size(data_size)
{
    if(priority < AppConfig::sending_events_offset)
    {
        this->priority = AppConfig::sending_events_offset;
    }
    else
    {
        this->priority = priority;
    }
}

int Event::bufferSize()
{
    return event_datagram_size;
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
//    return out << "EventType: " << e.type << " FrameNum: " << e.frame_number.l_value << " Size: " << e.event_datagram_size;
}

//================================================

KeyEvent::KeyEvent() :
    Event(KEY_EVENT_TYPE, 6, 7),
    key_type(NONE)
{
}

void KeyEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];

    key_type = static_cast<KeyEvent::KeyType>(buffer[index++]);

    id_tank.c_value[0] = buffer[index++];
    id_tank.c_value[1] = buffer[index++];
    id_tank.c_value[2] = buffer[index++];
    id_tank.c_value[3] = buffer[index++];
}

char *KeyEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = key_type;

    buffer[index++] = id_tank.c_value[0];
    buffer[index++] = id_tank.c_value[1];
    buffer[index++] = id_tank.c_value[2];
    buffer[index++] = id_tank.c_value[3];

    return buffer;
}

//================================================

GenerateTankEvent::GenerateTankEvent() : Event(GENERATE_TANK_EVENT_TYPE, 19)
{
}

void GenerateTankEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];

    tank_type = static_cast<GenerateTankEvent::TankType>(buffer[index++]);
    bonus = static_cast<GenerateTankEvent::Bonus>(buffer[index++]);

    for(int i = 0; i < 4; i++)
    {
        obj_id.c_value[i] = buffer[index++];
    }
    for(int i = 0; i < 4; i++)
    {
        lives.c_value[i] = buffer[index++];
    }

    for(int i = 0; i < 4; i++)
    {
        pos_x.c_value[i] = buffer[index++];
    }

    for(int i = 0; i < 4; i++)
    {
        pos_y.c_value[i] = buffer[index++];
    }
}

char *GenerateTankEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = tank_type;
    buffer[index++] = bonus;

    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = obj_id.c_value[i];
    }
    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = lives.c_value[i];
    }

    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = pos_x.c_value[i];
    }

    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = pos_y.c_value[i];
    }

    return buffer;
}

PlayerNameEvent::PlayerNameEvent() : Event(PLAYER_ID_TYPE, 21)
{
}

void PlayerNameEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];
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
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = player_id.c_value[0];
    buffer[index++] = player_id.c_value[1];
    buffer[index++] = player_id.c_value[2];
    buffer[index++] = player_id.c_value[3];

    for(int i = 0; i < 15; i++)
    {
        buffer[index++] = name[i];
    }

    return buffer;
}

InitEvent::InitEvent() : Event(INIT_EVENT_TYPE, 10)
{

}

void InitEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];
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
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = current_frame.c_value[0];
    buffer[index++] = current_frame.c_value[1];
    buffer[index++] = current_frame.c_value[2];
    buffer[index++] = current_frame.c_value[3];

    buffer[index++] = player_id.c_value[0];
    buffer[index++] = player_id.c_value[1];
    buffer[index++] = player_id.c_value[2];
    buffer[index++] = player_id.c_value[3];

    return buffer;
}

DisconnectEvent::DisconnectEvent() : Event(DISCONNECT_EVENT_TYPE, 6)
{

}

void DisconnectEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];
    index++;    //no sepc type

    player_id.c_value[0] = buffer[index++];
    player_id.c_value[1] = buffer[index++];
    player_id.c_value[2] = buffer[index++];
    player_id.c_value[3] = buffer[index++];
}

char *DisconnectEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    buffer[index++] = player_id.c_value[0];
    buffer[index++] = player_id.c_value[1];
    buffer[index++] = player_id.c_value[2];
    buffer[index++] = player_id.c_value[3];

    return buffer;
}

StartGameEvent::StartGameEvent() : Event(START_GAME_TYPE, 2, 30)
{

}

void StartGameEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];
    index++;    //no sepc type
}

char *StartGameEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = 0;

    return buffer;
}

PositionEvent::PositionEvent() : Event(POSITION_TYPE, 6 + 2 * sizeof(double), 1)
{

}

void PositionEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];

    obj = static_cast<PositionEvent::PosObj>(buffer[index++]);

    for(int i = 0; i < 4; i++)
    {
        obj_id.c_value[i] = buffer[index++];
    }

    for(int i = 0; i < sizeof(double); i++)
    {
        pos_x.c_value[i] = buffer[index++];
    }

    for(int i = 0; i < sizeof(double); i++)
    {
        pos_y.c_value[i] = buffer[index++];
    }
}

char *PositionEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = obj;

    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = obj_id.c_value[i];
    }

    for(int i = 0; i < sizeof(double); i++)
    {
        buffer[index++] = pos_x.c_value[i];
    }

    for(int i = 0; i < sizeof(double); i++)
    {
        buffer[index++] = pos_y.c_value[i];
    }

    return buffer;
}

SpeedChangeEvent::SpeedChangeEvent() : Event(SPEED_CHANGE_TYPE, 2, 3)
{

}

void SpeedChangeEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];
    speed_change_type = static_cast<SpeedChangeType>(buffer[index++]);    //no sepc type
}

char *SpeedChangeEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;
//    buffer[index++] = frame_number.c_value[0];
//    buffer[index++] = frame_number.c_value[1];
//    buffer[index++] = frame_number.c_value[2];
//    buffer[index++] = frame_number.c_value[3];

    buffer[index++] = speed_change_type;

    return buffer;
}

GenerateBonusEvent::GenerateBonusEvent() : Event(GENERATE_BONUS_EVENT_TYPE, 14)
{
}

void GenerateBonusEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];

    bonus = static_cast<GenerateBonusEvent::BonusType>(buffer[index++]);

    for(int i = 0; i < 4; i++)
    {
        obj_id.c_value[i] = buffer[index++];
    }
    for(int i = 0; i < 4; i++)
    {
        pos_x.c_value[i] = buffer[index++];
    }
    for(int i = 0; i < 4; i++)
    {
        pos_y.c_value[i] = buffer[index++];
    }
}

char *GenerateBonusEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;

    buffer[index++] = bonus;

    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = obj_id.c_value[i];
    }
    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = pos_x.c_value[i];
    }
    for(int i = 0; i < 4; i++)
    {
        buffer[index++] = pos_y.c_value[i];
    }

    return buffer;
}

LevelStateEvent::LevelStateEvent() : Event(LEVEL_STATE_TYPE, 6)
{

}

void LevelStateEvent::setByteArray(char *buffer)
{
    int index = 1;

//    frame_number.c_value[0] = buffer[index++];
//    frame_number.c_value[1] = buffer[index++];
//    frame_number.c_value[2] = buffer[index++];
//    frame_number.c_value[3] = buffer[index++];

    levelType = static_cast<LevelStateEvent::LevelType>(buffer[index++]);

    pos_r = buffer[index++];
    pos_c = buffer[index++];
    brick_collision_count = buffer[index++];
    brick_state_code = buffer[index++];
}

char *LevelStateEvent::getByteArray()
{
    char* buffer = new char[event_datagram_size];
    int index = 0;
    buffer[index++] = type;

    buffer[index++] = levelType;

    buffer[index++] = pos_r;
    buffer[index++] = pos_c;
    buffer[index++] = brick_collision_count;
    buffer[index++] = brick_state_code;

    return buffer;
}
