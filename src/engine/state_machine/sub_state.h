#ifndef ENGINE_STATE_MACHINE_SUB_STATE_H
#define ENGINE_STATE_MACHINE_SUB_STATE_H

#include "state.h"

template <typename T>
class SubState : public State
{
public:
    SubState(T *parent, StateMachine* stateMachine)
        : State(stateMachine), m_parent_state(parent)
    {
    }
    ~SubState() override = default;

    virtual void draw(Renderer &renderer) override {};
    virtual void update(const UpdateState &updateState) override {};
    virtual void eventProcess(const Event &event) override {};

protected:
    T *m_parent_state;
};

#endif // ENGINE_STATE_MACHINE_SUB_STATE_H