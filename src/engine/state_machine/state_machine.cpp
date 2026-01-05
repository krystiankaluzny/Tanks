#include "state_machine.h"
#include <iostream>

StateMachine::StateMachine()
    : current_state(nullptr),
      m_running(true),
      m_sub_state_machine(nullptr),
      m_sub_state_machine_owner(nullptr)
{
}

StateMachine::~StateMachine()
{
    m_sub_state_machine_owner = nullptr;

    if (m_sub_state_machine != nullptr)
        delete m_sub_state_machine;
    if (current_state != nullptr)
        delete current_state;
}

void StateMachine::setState(State *new_state, bool force)
{
    if (current_state != new_state || force)
    {
        // Some states can set substate in their constructor, before setting itself as current_state
        // So if the new_state is the owner of existing substate machine, we should not delete it
        if (new_state != m_sub_state_machine_owner && m_sub_state_machine != nullptr)
        {
            // Delete m_sub_state_machine should be invok in draw/update/eventProcess if they are in the middle of processing
            m_sub_state_machine = nullptr;
            m_sub_state_machine_owner = nullptr;
        }

        current_state = new_state;
    }
}

void StateMachine::stop()
{
    m_running = false;
}

bool StateMachine::stopped() const
{
    return !m_running;
}

StateMachine *StateMachine::subStateMachine(State *owner)
{
    if (m_sub_state_machine == nullptr)
    {
        m_sub_state_machine = new StateMachine();
    }
    m_sub_state_machine_owner = owner;
    return m_sub_state_machine;
}

void StateMachine::draw(Renderer &renderer)
{
    if (!m_running)
        return;

    State *state = current_state;
    StateMachine *sub_state_machine = m_sub_state_machine;

    if (state != nullptr)
    {
        state->draw(renderer);

        if (m_sub_state_machine == sub_state_machine && sub_state_machine != nullptr)
        {
            m_sub_state_machine->draw(renderer);
        }

        if (sub_state_machine != m_sub_state_machine && sub_state_machine != nullptr)
        {
            delete sub_state_machine;
        }

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
    StateMachine *sub_state_machine = m_sub_state_machine;

    if (state != nullptr)
    {
        state->update(updateState);

        if (m_sub_state_machine == sub_state_machine && sub_state_machine != nullptr)
        {
            sub_state_machine->update(updateState);
        }

        if (sub_state_machine != m_sub_state_machine && sub_state_machine != nullptr)
        {
            delete sub_state_machine;
        }

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
    StateMachine *sub_state_machine = m_sub_state_machine;

    if (state != nullptr)
    {
        state->eventProcess(event);
        if (m_sub_state_machine == sub_state_machine && sub_state_machine != nullptr)
        {
            m_sub_state_machine->eventProcess(event);
        }

        if (sub_state_machine != m_sub_state_machine && sub_state_machine != nullptr)
        {
            delete sub_state_machine;
        }

        if (state != current_state)
        {
            delete state;
        }
    }
}
