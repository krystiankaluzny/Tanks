#include "state.h"
#include "state_machine.h"

State::State(StateMachine *parent_state_machine)
    : m_state_machine(parent_state_machine)
{
}

State::~State()
{
    m_state_machine = nullptr;
}

StateMachine* State::stateMachine() const
{
    return m_state_machine;
}

void State::transiteTo(State *new_state, bool force)
{
    m_state_machine->setState(new_state, force);
}

void State::transiteToStop()
{
    m_state_machine->setState(nullptr);
    m_state_machine->stop();
}

void State::setSubState(State *new_state, bool force)
{
    m_state_machine->subStateMachine(this)->setState(new_state, force);
}