#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL2/SDL_events.h>

class AppState
{
public:
    virtual bool finished() const = 0;
    virtual void draw() = 0;
    virtual void update(Uint32 dt) = 0;
    virtual void eventProces(SDL_Event* ev) = 0;
    virtual AppState* nextState() = 0;
};

#endif // APPSTATE_H
