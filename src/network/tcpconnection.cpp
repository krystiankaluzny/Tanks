#include "tcpconnection.h"

#include "network.h"

TCPConnection::TCPConnection(){}

void TCPConnection::setParent(Network *parent)
{
    this->parent = parent;
}
