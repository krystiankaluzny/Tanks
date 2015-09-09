#ifndef EVENT_H
#define EVENT_H

#include <iostream>

enum EventType
{
    NONE_EVENT_TYPE,
    KEY_EVENT_TYPE,
    GENERATE_TANK_EVENT_TYPE,
    GENERATE_BONUS_EVENT_TYPE,
    PLAYER_ID_TYPE,
    INIT_EVENT_TYPE,
    DISCONNECT_EVENT_TYPE,
    START_GAME_TYPE,
    POSITION_TYPE,
    SPEED_CHANGE_TYPE,
    LEVEL_STATE_TYPE
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

union DoubleData
{
  double d_value;
  char c_value[sizeof(double)];
};

class Event
{
public:
    Event();
    Event(EventType type, int data_size, int priority = 20);

    EventType type;                 //1 byte

    const int event_datagram_size;
    int priority;
    virtual void setByteArray(char* buffer) = 0;
    virtual char *getByteArray() = 0;
    int bufferSize();
    void setZeroPos(char *buff, int starting_point);
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

class GenerateTankEvent : public Event
{
public:
    enum TankType
    {
        A = 0, B, C, D,
    };
    enum Bonus
    {
        YES,
        NO
    };

    GenerateTankEvent();

    TankType tank_type; // 1 byte spec
    Bonus bonus; // 1 byte spec
    LongData obj_id;  //4 byte
    LongData lives;  //4 byte
    LongData pos_x; //4 byte
    LongData pos_y; //4 byte

    void setByteArray(char *buffer);
    char* getByteArray();
};

class GenerateBonusEvent : public Event
{
public:
    enum BonusType
    {
        BONUS_GRANATE = 0,
        BONUS_HELMET,
        BONUS_CLOCK,
        BONUS_SHOVEL,
        BONUS_TANK,
        BONUS_STAR,
        BONUS_GUN,
        BONUS_BOAT
    };

    GenerateBonusEvent();

    BonusType bonus; // 1 byte spec
    LongData obj_id;  //4 byte
    LongData pos_x; //4 byte
    LongData pos_y; //4 byte

    void setByteArray(char *buffer);
    char* getByteArray();
};

class PlayerNameEvent : public Event
{
public:
    PlayerNameEvent();
    //1 byte spec
    LongData player_id; //4 byte
    char name[15];      //15 byte
    void setByteArray(char *buffer);
    char* getByteArray();
};

class InitEvent : public Event
{
public:
    InitEvent();
    //1 byte spec
    LongData current_frame; //4 byte
    LongData player_id; //4 byte
    void setByteArray(char *buffer);
    char* getByteArray();
};

class DisconnectEvent : public Event
{
public:
    DisconnectEvent();
    //1 byte spec
    LongData player_id; //4 byte
    void setByteArray(char *buffer);
    char* getByteArray();
};

class StartGameEvent : public Event
{
public:
    StartGameEvent();
    //1byte spec
    void setByteArray(char *buffer);
    char* getByteArray();
};

class PositionEvent : public Event
{
public:
    enum PosObj
    {
        TANK
    };

    PositionEvent();

    PosObj obj; //1 byte spec
    LongData obj_id; //4 byte
    DoubleData pos_x; //8 byte
    DoubleData pos_y; //8 byte
    void setByteArray(char *buffer);
    char* getByteArray();
};

class SpeedChangeEvent : public Event
{
public:
    enum SpeedChangeType
    {
        SLOW_DOWN,
        SPEED_UP
    };
    SpeedChangeEvent();

    SpeedChangeType speed_change_type;
    void setByteArray(char *buffer);
    char* getByteArray();
};

class LevelStateEvent : public Event
{
public:
    enum LevelType
    {
        BRICK,
        STONE,
        NONE
    };
    LevelStateEvent();

    LevelType levelType; //1 byte
    unsigned char pos_r; //1 byte //row
    unsigned char pos_c; //1 byte //column
//    unsigned char brick_collision_count; //1 byte
//    unsigned char brick_state_code; //1 byte

    void setByteArray(char *buffer);
    char* getByteArray();
};

//TODO next stage
#endif // EVENT_H
