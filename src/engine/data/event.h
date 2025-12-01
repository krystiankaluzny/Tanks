#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "keycode.h"

class Event
{
public:
    enum EventType
    {
        UNKNOWN,
        KEYBOARD,
        WINDOW,
        QUIT
    };
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
    KeyboardEvent(KeyCode keyCode, unsigned int keyMods)
        : Event(EventType::KEYBOARD), keyCode(keyCode), keyMods(keyMods) {};

    KeyCode getKeyCode() const { return keyCode; }
    bool testKeyMod(KeyMod keyMod) const { return keyMods & static_cast<unsigned int>(keyMod); }
};

class WindowEvent : public Event
{
public:
    enum WindowEventType
    {
        SHOWN,
        HIDDEN,
        EXPOSED,
        MOVED,
        RESIZED,
        SIZE_CHANGED,
        MINIMIZED,
        MAXIMIZED,
        RESTORED,
        ENTER,
        LEAVE,
        FOCUS_GAINED,
        FOCUS_LOST,
        CLOSE,
        UNKNOWN
    };

    WindowEvent(WindowEventType type, Size window_size)
        : Event(EventType::WINDOW), type(type), window_size(window_size) {};

    WindowEventType type;
    Size window_size;
};

#endif // TANKS_EVENT_H