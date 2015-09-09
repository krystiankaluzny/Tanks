#include "appthread.h"


AppThread::AppThread(SharedData *shared_data, CRITICAL_SECTION *critical_section)
{
    this->shared_data = shared_data;
    this->critical_section = critical_section;
}

boolean AppThread::isNetworkRunning()
{
    NetworkState state;
    EnterCriticalSection(critical_section);
        state = shared_data->network_state;
    LeaveCriticalSection(critical_section);

    return state != NetworkState::NONE;
}
