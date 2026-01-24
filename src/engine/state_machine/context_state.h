#ifndef ENGINE_STATE_MACHINE_CONTEXT_STATE_H
#define ENGINE_STATE_MACHINE_CONTEXT_STATE_H

#include "state.h"

template <typename T>
class ContextState : public State
{
public:
    ContextState(T *context, StateMachine* stateMachine)
        : State(stateMachine), m_context(context)
    {
    }
    ~ContextState() override = default;

    virtual void draw(Renderer &renderer) override {};
    virtual void update(const UpdateState &updateState) override {};
    virtual void eventProcess(const Event &event) override {};

protected:
    T *m_context;
};

#endif // ENGINE_STATE_MACHINE_CONTEXT_STATE_H