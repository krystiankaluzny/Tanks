#ifndef ENGINE_STATE_MACHINE_STATE_H
#define ENGINE_STATE_MACHINE_STATE_H

#include "../data/event.h"
#include "../renderer.h"
#include "../engine.h"
#include "state_machine.h"

class StateMachine;

class State
{
public:
    virtual ~State();

    void initialize();

    virtual void draw(Renderer &renderer) = 0;
    virtual void update(const UpdateState &updateState) = 0;
    virtual void eventProcess(const Event &event) = 0;

    StateMachine* stateMachine() const;

protected:
    State(StateMachine* state_machine);

    virtual void onInitialize();

    void transiteTo(State* new_state, bool force = false);
    void transiteToNull();
    void transiteToNullAndStop();

    StateMachine* m_state_machine;
    bool m_initialized;
};

#endif // STATE_MACHINE_STATE_H