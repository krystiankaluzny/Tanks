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

