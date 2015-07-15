#ifndef EVENT_H
#define EVENT_H

#include <iostream>

enum EventType
{
    NONE_EVENT,
    COLLISION_EVENT,
    MOVE_EVENT,
    FIRE_EVENT,
    GENERATE_EVENT,
    BONUS_EVENT
};

union LongData
{
    unsigned long l_value;
    char c_value[4]; //c_value[0] - najmłodszy bajt l_value
    LongData() : l_value(0) {}
};

//
class Event
{
public:
    Event();
    Event(EventType type, int data_size);

    EventType type;
    LongData frame_number;

    const int event_datagram_size;

    virtual void fillData(char* buffer);
    friend std::ostream& operator<< (std::ostream& out, Event& e);
};

class CollisionEvent : public Event
{
public:
    enum CollisionType
    {
        NONE,
        TANK_TANK,
        BULLET_LEVEL,
        BULLET_BUSH,
        BULLET_ENEMY,
        BULLET_BULLET,
        BULLET_PLAYER,
        PLAYER_BONUS,
        TANK_LEVEL
    };

    CollisionEvent();

    CollisionType collision_type;
    LongData id_object1;
    LongData id_object2;

    void fillData(char *buffer);
    friend std::ostream& operator<< (std::ostream& out, CollisionEvent& e);
};

class MoveEvent : public Event
{
public:
    enum Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    MoveEvent();

    Direction move_direction;
    LongData id_tank;

    void fillData(char *buffer);
};

class FireEvent : public Event
{
public:
    FireEvent();

    LongData id_tank;

    void fillData(char *buffer);
};

class GenerateEvent : public Event
{
public:
    enum GenerateObject
    {
        NONE,
        BONUS,
        ENEMY
    };

    GenerateEvent();

    GenerateObject object_type;
    LongData generate_seed;

    void fillData(char *buffer);
};


class BonusEvent : public Event
{
public:
    enum BonusEventType
    {
        NONE,
        GRANATE_START,
        HELMET_START,
        HELMET_END,
        CLOCK_START,
        CLOCK_END,
        SHOVEL_START,
        SHOVEL_END,
        TANK_START,
        STAR_START,
        GUN_START,
        BOAT_START,
        BOAT_END
    };

    BonusEvent();

    BonusEventType bonus_type;
    LongData id_player;

    void fillData(char *buffer);
};

#endif // EVENT_H
