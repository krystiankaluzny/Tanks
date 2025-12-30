#ifndef APPSTATE_H
#define APPSTATE_H

#include "../engine/data/event.h"
#include "../engine/renderer.h"
#include "../engine/engine.h"


class AppState
{
public:
    virtual ~AppState() {}

    virtual void draw(Renderer &renderer) = 0;
    virtual void update(const UpdateState& updateState) = 0;
    virtual void eventProcess(const Event &event) = 0;
    virtual AppState* nextState() = 0;

protected:
    AppState(InteractiveComponents m_interactive_components);
    void playSound(const Sound &sound);
    void stopAllSounds();

    InteractiveComponents m_interactive_components;
};
#endif // APPSTATE_H
