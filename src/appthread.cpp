#include "appthread.h"


AppThread::AppThread(SharedData *shared_data, CRITICAL_SECTION *critical_section)
{
    this->shared_data = shared_data;
    this->critical_section = critical_section;
}

void AppThread::saveEvent(Event *event)
{

}

void AppThread::sendEventQueue()
{

}
