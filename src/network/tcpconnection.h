#ifndef TCPCONNECTION
#define TCPCONNECTION

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include "../type.h"
#include "../event/event.h"
#include "../appthread.h"

class Network;

class TCPConnection
{
public:
    TCPConnection();
    void setParent(Network* parent);

    /**
     * Inicjalizacja klienta lub serwera
     * @return
     */
    virtual bool init() = 0;
    /**
     * Pętla głowna klienta lub serwera.
     */
    virtual void run() = 0;
    /**
     * Zamknięcie połączenia
     */
    virtual void close() = 0;
    /**
     * Wysłanie danych.
     */
    virtual void sendData() = 0;
    /**
     * Odebranie danych.
     */
    virtual void readData() = 0;

protected:
    /**
     * Ustawienie nazwy gracza związanej z danym socketem.
     * @param player_socket
     * @param player_name
     */
    void setPlayerName(SOCKET player_socket, std::string player_name);
    /**
     * Usunięceie gracza z listy dostępnych.
     * @param player_socket
     */
    void removePlayerName(SOCKET player_socket);
    /**
     * Usunięcie wszystkich graczy.
     */
    void clearNames();
    /**
     * Parsowanie ramki odebranych danych.
     * @param buffer
     * @param size
     */
    void addEventFromBuffer(char* buffer, int size);

    /**
     * Wyświetlenie tablicy bytów jaką tekst
     * @param data
     * @param size
     */
    void printHex(char* data, int size);
    /**
     * Inicjalizacja klienta.
     * @param event
     */
    void initialize(InitEvent *event);

    /**
     * Rodzic dziękti, któremu można dostać się do danyh współdzielonych między wątkami.
     */
    Network* parent;


    /**
     * Czas oczekiwania na pojawienie się następnej wiadomości od serwera/klienta
     */
    const int wait_for_event_time = 10; //ms
    /**
     * Port do połączenia.
     */
    const u_short PORT = 4782;
    /**
     * Bufor na dane przychądzące.
     */
    char buffer[500];

    /**
     * @brief sockets
     */
    std::vector<SOCKET> sockets; //pierwszy socket jest socketem serwera jeśli włączono serwer
    std::vector<WSAEVENT> sockets_event;
    /**
     * Maksymalna liczba socketów (wraz serwerem)
     */
    const int socket_limit = 2;
};

#endif // TCPCONNECTION

