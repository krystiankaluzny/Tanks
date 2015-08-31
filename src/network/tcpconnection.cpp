#include "tcpconnection.h"

#include "network.h"
#include <stdio.h>

TCPConnection::TCPConnection(){}

void TCPConnection::setParent(Network *parent)
{
    this->parent = parent;
}

void TCPConnection::setPlayerName(SOCKET player_socket, string player_name)
{
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->player_name[player_socket] = player_name; //[] - dodaje gracza jeśli nie istaniał, 'at()' tego nie robi
    LeaveCriticalSection(parent->critical_section);
}

void TCPConnection::removePlayerName(SOCKET player_socket)
{
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->player_name.erase(player_socket);
    LeaveCriticalSection(parent->critical_section);
}

void TCPConnection::clearNames()
{
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->player_name.clear();
    LeaveCriticalSection(parent->critical_section);
}

void TCPConnection::addEventFromBuffer(char *buffer, int size)
{
    int index = 0;
    char event_type = buffer[index++];
    Event* event = nullptr;
    switch(event_type)
    {
        case GENERATE_EVENT_TYPE:
        {
            event = new GenerateEvent;
            break;
        }
        case PLAYER_ID_TYPE:
        {
            event = new PlayerNameEvent;
            break;
        }
        case INIT_EVENT_TYPE:
        {
            InitEvent* init = new InitEvent;
            if(init->bufferSize() != size) break;

            init->setByteArray(buffer);
            std::cout << "init" <<std::endl;
            initialize(init);
            std::cout << "init" <<std::endl;

            break;
        }
    default:
        std::cout << "NIE ROZPOZNANO " << event_type << std::endl;
    }



    if(event != nullptr)
    {
        if(event->bufferSize() != size)
        {
            std::cout << "event->bufferSize() != size" << event->bufferSize() << " " << size<< std::endl;
        }
        else
        {
            event->setByteArray(buffer);
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->received_events.addEvent(event);
            LeaveCriticalSection(parent->critical_section);
        }

    }
}

void TCPConnection::getLongData(LongData &event_index, LongData &events_count, char *buffer)
{
    int index = 0;
    event_index.c_value[0] = buffer[index++];
    event_index.c_value[1] = buffer[index++];
    event_index.c_value[2] = buffer[index++];
    event_index.c_value[3] = buffer[index++];

    events_count.c_value[0] = buffer[index++];
    events_count.c_value[1] = buffer[index++];
    events_count.c_value[2] = buffer[index++];
    events_count.c_value[3] = buffer[index++];
}

void TCPConnection::printHex(char *data, int size)
{
    printf("data_size %d \n", size);
    for(int i = 0; i < size; ++i)
    {
        printf("0x%02x ", data[i]);
    }
    printf("\n");
}

void TCPConnection::initialize(InitEvent* event)
{
    EnterCriticalSection(parent->critical_section);
        parent->shared_data->setCurrentFrameNumber(event->current_frame.l_value);
        parent->shared_data->player_id = event->player_id.l_value;
    LeaveCriticalSection(parent->critical_section);

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->clearReceiveEvents(-1);
        parent->shared_data->transmit_events.events.clear();
    LeaveCriticalSection(parent->critical_section);

    EnterCriticalSection(parent->critical_section);
        parent->shared_data->network_state = NetworkState::CLIENT_INITIALIZED;
    LeaveCriticalSection(parent->critical_section);
}

