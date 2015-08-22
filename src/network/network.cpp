#include "network.h"

Network::Network(SharedData *shared_data, CRITICAL_SECTION *critical_section) :
    AppThread(shared_data, critical_section)
{
    server.setParent(this);
    client.setParent(this);

    state = NetworkState::NONE;
    is_running = false;
}

void Network::run()
{
    is_running = true;
    while(is_running)
    {
        //GetShared Data
        NetworkState new_state;
        EnterCriticalSection(critical_section);
            new_state = shared_data->network_state;
            is_running = shared_data->run_app;
        LeaveCriticalSection(critical_section);

        if(state != new_state)
        {
            if(state == NetworkState::SERVER)
            {
                server.close();
            }
            else if(state == NetworkState::CLIENT)
            {
                //close client
            }

            if(new_state == NetworkState::SERVER)
            {
                if(server.init())
                {
                    state = new_state;
                }
                else
                {
                    state = NetworkState::NONE;
                }
            }
            else if(new_state == NetworkState::CLIENT)
            {
                if(client.init())
                {
                    state = new_state;
                }
                else
                {
                    state = NetworkState::NONE;
                }
            }
        }

        if(state == NetworkState::SERVER)
        {
            server.run();
        }
        else if(state == NetworkState::CLIENT)
        {
            client.run();
        }
        else
        {
            Sleep(7);
        }
        Sleep( 0 ); // reszta czasu dla drugiego wątku
    }
}

