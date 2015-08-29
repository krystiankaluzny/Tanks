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

void TCPConnection::addEventFromBuffer(char *buffer, int size)
{
    int index = 0;
    char event_type = buffer[index++];
    LongData event_index, events_count;
    switch(event_type)
    {
        case GENERATE_EVENT_TYPE:
        {
            GenerateEvent* event = new GenerateEvent;
            if(event->bufferSize() != size) break;

            getLongData(event_index, events_count, buffer + event->event_datagram_size); //dwa longi z końca ramki
            event->setByteArray(buffer);
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->received_events.addEvent(event, event_index.l_value, events_count.l_value);
            LeaveCriticalSection(parent->critical_section);

            break;
        }
        case PLAYER_ID_TYPE:
        {
            PlayerIdEvent* event = new PlayerIdEvent;
            if(event->bufferSize() != size) break;

            getLongData(event_index, events_count, buffer + event->event_datagram_size); //dwa longi z końca ramki
            event->setByteArray(buffer);
            EnterCriticalSection(parent->critical_section);
                parent->shared_data->received_events.addEvent(event, event_index.l_value, events_count.l_value);
            LeaveCriticalSection(parent->critical_section);
            break;
        }
        case INIT_EVENT_TYPE:
        {
            InitEvent* event = new InitEvent;
            if(event->bufferSize() != size) break;

            getLongData(event_index, events_count, buffer + event->event_datagram_size); //dwa longi z końca ramki
            event->setByteArray(buffer);
            std::cout << "Init: Old "  << parent->getCurrentFrame() << std::endl;

            EnterCriticalSection(parent->critical_section);
                parent->shared_data->setCurrentFrameNumber(event->current_frame.l_value);
            LeaveCriticalSection(parent->critical_section);

            std::cout << "Init: New "  << parent->getCurrentFrame() << std::endl;
            break;
        }
    default:
        std::cout << "NIE ROZPOZNANO " << event_type << std::endl;
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

