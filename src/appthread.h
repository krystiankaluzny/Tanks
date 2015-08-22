#ifndef APPTHREAD
#define APPTHREAD
#include "scopelock.h"
#include "event/eventstore.h"
#include "shareddata.h"

#include <process.h>
#include <windows.h>

/**
 * @brief
 * Klasa bazowa dla dwóch wątków w aplikacji: gry i sieci.
 */
class AppThread
{
public:
    AppThread(SharedData* shared_data, CRITICAL_SECTION* critical_section);

    /**
     * Uruchomienie działania wątku.
     */
    virtual void run() = 0;

protected:
    /**
     * Zapisanie eventu do kolejki, która ma zostać wysłana na server.
     * @param event
     */
    void saveEvent(Event* event);

    /**
     * Ustawienie flagi wysłania eventów na server.
     */
    void sendEventQueue();

    /**
     * Dane współdzielone między wątkami.
     */
    SharedData* shared_data;

    /**
     * Blokada wątku wykorzystywana przy dostępie do współdzielonych danych.
     */
    CRITICAL_SECTION* critical_section;
};

#endif // APPTHREAD

