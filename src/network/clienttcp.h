#ifndef CLENTTCP_H
#define CLENTTCP_H

#include "tcpconnection.h"
#include "vector"

using namespace std;

class ClientTCP : public TCPConnection
{
public:
    ClientTCP();
    ~ClientTCP();

    /**
     * inicjalizacja klienta TCP
     * @return
     */
    bool init();
    /**
     * Pętla głowna klienta
     */
    void run();
    /**
     * Zamknięcie klienta TCP.
     */
    void close();
    /**
     * Wysłanie danych do serwera
     */
    void sendData();
    /**
     * Odczyt danych z serwera
     */
    void readData();
    /**
     * Nazwa lub adres ip serwera
     */
    string hostName;
};

#endif // CLENTTCP_H
