#include "state_machine.h"
#include <iostream>

StateMachine::StateMachine()
    : current_state(nullptr),
      m_running(true)
{
}

StateMachine::~StateMachine()
{
    if (current_state != nullptr)
        delete current_state;
}

void StateMachine::setState(State *new_state, bool force)
{
    if (current_state != new_state || force)
    {
        current_state = new_state;
    }
}

void StateMachine::resetState(State *new_state)
{
    if (current_state != nullptr)
    {
        delete current_state;
    }
    current_state = new_state;
}

void StateMachine::stop()
{
    m_running = false;
}

bool StateMachine::stopped() const
{
    return !m_running;
}

void StateMachine::draw(Renderer &renderer)
{
    if (!m_running)
        return;

    State *state = current_state;

    if (state != nullptr)
    {
        state->draw(renderer);

        if (state != current_state)
        {
            delete state;
        }
    }
}

void StateMachine::update(const UpdateState &updateState)
{
    if (!m_running)
        return;

    State *state = current_state;

    if (state != nullptr)
    {
        state->update(updateState);

        if (state != current_state)
        {
            delete state;
        }
    }
}

void StateMachine::eventProcess(const Event &event)
{
    if (!m_running)
        return;

    State *state = current_state;

    if (state != nullptr)
    {
        state->eventProcess(event);

        if (state != current_state)
        {
            delete state;
        }
    }
}
