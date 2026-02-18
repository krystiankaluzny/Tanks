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
    initializeStateIfNeeded(state);

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
    initializeStateIfNeeded(state);

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
    initializeStateIfNeeded(state);

    if (state != nullptr)
    {
        state->eventProcess(event);

        if (state != current_state)
        {
            delete state;
        }
    }
}

void StateMachine::initializeStateIfNeeded(State *state)
{
    if (state != nullptr)
    {
        /*
            NOTE: State can transit to another state during processing,
            so initialization of game objects should not be done in a constructor of the new state,
            but rather in a separate method, which will be called before processing the state for the first time.

            state->initialize() should handle multiple calls, so it is safe to call it every time when state is processed.
        */

        state->initialize();
    }
}