
#include "event.h"
#include "keycode.h"

class QuitEvent : public Event
{
public:
    QuitEvent();
};

QuitEvent::QuitEvent() : Event(EventType::QUIT)
{
}

class KeyboardEvent : public Event
{
private:
    KeyCode keyCode;
    unsigned int keyMods;

public:
    KeyboardEvent();


    KeyCode getKeyCode() const { return keyCode; }
    bool testKeyMod(KeyMod keyMod) const { return keyMods & static_cast<unsigned int>(keyMod); }
};

KeyboardEvent::KeyboardEvent() : Event(EventType::KEYDOWN)
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

