#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "keycode.h"

enum EventType
{
    UNKNOWN,
    KEYBOARD,
    WINDOW,
    QUIT
};

class Event
{
public:
    EventType type;

protected:
    Event(EventType type)
    {
        this->type = type;
    }
};

class UnknownEvent : public Event
{
public:
    UnknownEvent() : Event(EventType::UNKNOWN) {};
};

class QuitEvent : public Event
{
public:
    QuitEvent() : Event(EventType::QUIT) {};
};

class KeyboardEvent : public Event
{
private:
    KeyCode keyCode;
    unsigned int keyMods;

public:
    KeyboardEvent() : Event(EventType::KEYBOARD) {};

    KeyCode getKeyCode() const { return keyCode; }
    bool testKeyMod(KeyMod keyMod) const { return keyMods & static_cast<unsigned int>(keyMod); }
};

class WindowEvent : public Event
{
public:
    WindowEvent() : Event(EventType::WINDOW) {};
};

#endif // TANKS_EVENT_H