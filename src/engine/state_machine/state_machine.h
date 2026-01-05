#ifndef ENGINE_STATE_MACHINE_H
#define ENGINE_STATE_MACHINE_H

#include "state.h"

class State;

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void setState(State *new_state, bool force = false);
    void stop();
    bool stopped() const;
    StateMachine* subStateMachine(State* owner);

    void draw(Renderer &renderer);
    void update(const UpdateState &updateState);
    void eventProcess(const Event &event);

    State *current_state;

private:
    bool m_running;
    StateMachine *m_sub_state_machine;
    State* m_sub_state_machine_owner;
};

#endif // STATE_MACHINE_H