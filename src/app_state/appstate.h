#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL2/SDL_events.h>

class AppState
{
public:
    virtual void draw() = 0;
    virtual void update(Uint32 dt) = 0;
    virtual void eventProces(SDL_Event* ev) = 0;
};

#endif // APPSTATE_H
