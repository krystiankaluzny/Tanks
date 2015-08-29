#ifndef EVENT_H
#define EVENT_H

#include <iostream>

enum EventType
{
    NONE_EVENT_TYPE,
    KEY_EVENT_TYPE,
    GENERATE_EVENT_TYPE,
    PLAYER_ID_TYPE,
    INIT_EVENT_TYPE
};

union LongData
{
    unsigned long l_value;
    char c_value[4]; //c_value[0] - najmłodszy bajt l_value
    LongData() : l_value(0) {}
};

union IntData
{
    unsigned int i_value;
    char c_value[2]; //c_value[0] - najmłodszy bajt i_value
    IntData() : i_value(0) {}
};



class Event
{
public:
    Event();
    Event(EventType type, int data_size);

    EventType type;                 //1 byte
    LongData frame_number;          //4 byte

    const int event_datagram_size;

    virtual void setByteArray(char* buffer) = 0;
    virtual char *getByteArray() = 0;
    int bufferSize();
    friend std::ostream& operator<< (std::ostream& out, Event& e);
};

class KeyEvent : public Event
{
public:
    enum KeyType
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT,

        FIRE,
        PAUSE
    };
    KeyEvent();

    KeyType key_type;   //1 byte
    LongData id_tank;   //4 byte

    void setByteArray(char *buffer);
    char* getByteArray();
};

class GenerateEvent : public Event
{
public:
    GenerateEvent();

    LongData seed1; //4 byte
    LongData seed2; //4 byte
    LongData seed3; //4 byte

    void setByteArray(char *buffer);
    char* getByteArray();
};

class PlayerIdEvent : public Event
{
public:
    PlayerIdEvent();

    LongData player_id; //4 byte
    char name[15];      //15 byte
    void setByteArray(char *buffer);
    char* getByteArray();
};

class InitEvent : public Event
{
public:
    InitEvent();

    LongData current_frame;

    void setByteArray(char *buffer);
    char* getByteArray();
};

//TODO next stage
#endif // EVENT_H
