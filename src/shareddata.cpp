#include "shareddata.h"

SharedData::SharedData()
{
    network_state = NetworkState::NONE;
    run_app = true;
    send_events = false;
    current_frame_number = 0;
}

void SharedData::clearEvents()
{
    //TODO usunąć ewenty z aktualnej ramki
}

void SharedData::setCurrentFrameNumber(unsigned long current_frame)
{
    current_frame_number = current_frame;
    received_events.current_frame = current_frame;
}

