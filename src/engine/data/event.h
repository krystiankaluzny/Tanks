#ifndef TANKS_EVENT_H
#define TANKS_EVENT_H

#include "keycode.h"
#include "data.h"

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

    EventType type() const { return m_type; }

protected:
    Event(EventType type) : m_type(type) {};

private:
    EventType m_type;
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
public:
    enum KeyState
    {
        PRESSED,
        RELEASED
    };

    KeyboardEvent(KeyCode key_code, KeyState key_state, unsigned int key_mods)
        : Event(EventType::KEYBOARD), m_key_code(key_code), m_key_state(key_state), m_key_mods(key_mods) {};

    bool isPressed(KeyCode code) const { return (m_key_state == PRESSED) && (m_key_code == code); }
    bool isReleased(KeyCode code) const { return (m_key_state == RELEASED) && (m_key_code == code); }

    KeyCode keyCode() const { return m_key_code; }
    KeyState keyState() const { return m_key_state; }
    bool testKeyMod(KeyMod keyMod) const { return m_key_mods & static_cast<unsigned int>(keyMod); }

private:
    KeyCode m_key_code;
    KeyState m_key_state;
    unsigned int m_key_mods;
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
        : Event(EventType::WINDOW), m_window_event_type(type), m_window_size(window_size) {};

    WindowEventType windowEventType() const { return m_window_event_type; }
    Size windowSize() const { return m_window_size; }

private:
    WindowEventType m_window_event_type;
    Size m_window_size;
};

#endif // TANKS_EVENT_H