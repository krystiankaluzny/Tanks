
#include "event.h"

class QuitEvent : public Event
{
public:
    QuitEvent();
};

QuitEvent::QuitEvent() : Event(EventType::QUIT)
{
}

class KeyDownEvent : public Event
{
private:
    /* data */
public:
    KeyDownEvent();
};

KeyDownEvent::KeyDownEvent() : Event(EventType::KEYDOWN)
{
}

class WindowEvent : public Event
{
public:
    WindowEvent();
};

WindowEvent::WindowEvent() : Event(EventType::WINDOWEVENT)
{
}

