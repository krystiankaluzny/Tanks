#include "shareddata.h"

SharedData::SharedData()
{
    network_state = NetworkState::NONE;
    run_app = true;
    send_events = false;
    current_frame_number = 0;
}

