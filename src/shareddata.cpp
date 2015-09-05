#include "shareddata.h"

SharedData::SharedData()
{
    network_state = NetworkState::NONE;
    run_app = true;
    send_events = false;
    current_frame_number = 0;
}

void SharedData::clearReceiveEvents(unsigned long frame)
{
    received_events.clearFrameEvents(frame);
}

void SharedData::clearCurrentEvents()
{
    clearReceiveEvents(current_frame_number);
}

void SharedData::setCurrentFrameNumber(unsigned long current_frame)
{
    current_frame_number = current_frame;
    received_events.current_frame = current_frame;
}

void SharedData::incrementFrameNumber()
{
    current_frame_number++;
    received_events.current_frame++;
}

void SharedData::newEvent(Event *event)
{
    std::cout << "new event " << event->frame_number.l_value << std::endl;

    if(network_state == SERVER)
    {
        received_events.addEvent(event);
    }
    else
    {
        transmit_events.addEvent(event);
    }
}

