#ifndef SERVERTCP_H
#define SERVERTCP_H

//#define _WIN32_WINNT 0x501

#include <iostream>
#include <vector>
#include <map>

#include "tcpconnection.h"

using namespace std;

class ServerTCP : public TCPConnection
{
public:
    ServerTCP();
    ~ServerTCP();

    /**
     * Pętla główna serwera.
     */
    void run();
    /**
     * Inicjalizacja serwera
     * @return
     */
    bool init();
    /**
     * Zamknięcie serwera
     */
    void close();
    /**
     * Wysłanie danych
     */
    void sendData();
    /**
     * Odbiór danych.
     */
    void readData();

private:

    /*
     * Aceptowanie prośby od klienta o połączenie (dodanie nowego socketu)
     */
    void acceptSocket();
    /*
     * Zamknięcie socketu
     */
    void closeSocket(int socket_index);
    /*
     * Ddczyt ramki danych z socketu
     */
    void readSocket(int socket_index);

    /**
     * Wusłanie ramki danych do wszystkich dołączonych klientów.
     * @param buf
     * @param size
     */
    void broadcast(char* buf, int size);
    /**
     * Wysłanie ramki inicjalizującej do wybranego klienta
     * @param s
     */
    void sendInit(SOCKET s);
};

#endif // SERVERTCP_H
