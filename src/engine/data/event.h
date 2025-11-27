#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

enum EventType
{
    KEYDOWN,
    WINDOWEVENT,
    QUIT
};

class Event
{
    public:
        EventType type;

    protected:
        Event(EventType type);
};


Event::Event(EventType ctype)
{
    type = ctype;
};

#endif //TANKS_EVENT_H