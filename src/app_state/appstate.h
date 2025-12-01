#ifndef APPSTATE_H
#define APPSTATE_H

#include "../engine/data/event.h"
#include "../engine/renderer.h"

class AppState
{
public:
    virtual ~AppState() {}

    virtual void draw(Renderer &renderer) = 0;
    virtual void update(Uint32 dt) = 0;
    virtual void eventProcess(const Event &event) = 0;
    virtual AppState* nextState() = 0;
};
#endif // APPSTATE_H
