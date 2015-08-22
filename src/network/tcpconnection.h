#ifndef TCPCONNECTION
#define TCPCONNECTION

class Network;

class TCPConnection
{
public:
    TCPConnection(){}
    void setParent(Network* parent)
    {
        this->parent = parent;
    }

protected:
    Network* parent;
};

#endif // TCPCONNECTION

