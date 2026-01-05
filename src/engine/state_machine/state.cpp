#include "state.h"
#include "state_machine.h"

State::State(StateMachine *state_machine)
    : m_state_machine(state_machine)
{
}

State::~State()
{
    m_state_machine = nullptr;
}

StateMachine *State::stateMachine() const
{
    return m_state_machine;
}

void State::transiteTo(State *new_state, bool force)
{
    m_state_machine->setState(new_state, force);
}

void State::transiteToNull()
{
    m_state_machine->setState(nullptr);
}

void State::transiteToNullAndStop()
{
    m_state_machine->setState(nullptr);
    m_state_machine->stop();
}