#ifndef APPSTATE_H
#define APPSTATE_H

#include "../engine/data/event.h"
#include "../engine/renderer.h"
#include "../engine/engine.h"
#include "../engine/state_machine/state.h"

class AppState : protected InteractiveComponentsHolder, public State
{
public:
    virtual ~AppState() {}

protected:
    AppState(InteractiveComponents m_interactive_components, StateMachine* parent_state_machine)
        : InteractiveComponentsHolder(m_interactive_components)
        , State(parent_state_machine)
        {}
};
#endif // APPSTATE_H
