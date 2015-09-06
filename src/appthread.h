#ifndef APPTHREAD
#define APPTHREAD
#include "scopelock.h"
#include "event/eventstore.h"
#include "shareddata.h"
#include "game/objects/object.h"

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
     * Czy gra jest w sieci.
     * @return
     */
    boolean isNetworkRunning();

    /**
     * Dane współdzielone między wątkami.
     */
    SharedData* shared_data;

    /**
     * Blokada wątku wykorzystywana przy dostępie do współdzielonych danych.
     */
    CRITICAL_SECTION* critical_section;

    unsigned long getCurrentFrame()
    {
        unsigned long frame;
        EnterCriticalSection(critical_section);
            frame = shared_data->current_frame_number;
        LeaveCriticalSection(critical_section);

        return frame;
    }

    unsigned long getPlayerId()
    {
        unsigned long player_id;
        EnterCriticalSection(critical_section);
            player_id = shared_data->player_id;
        LeaveCriticalSection(critical_section);

        return player_id;
    }

    void sendObjectPosition(Object* obj, PositionEvent::PosObj obj_type)
    {
        PositionEvent* pos = new PositionEvent;
        pos->frame_number.l_value = getCurrentFrame() + pos->priority;
        pos->obj = obj_type;
        pos->pos_x.d_value = obj->pos_x;
        pos->pos_y.d_value = obj->pos_y;
        EnterCriticalSection(critical_section);
            shared_data->newEvent(pos);
        LeaveCriticalSection(critical_section);
    }
};

#endif // APPTHREAD

