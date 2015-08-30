#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL2/SDL_events.h>
#include <string>

#include "../../event/event.h"
#include "../../type.h"
#include "../game.h"
//class Game;
/**
 * @brief
 * Klasa jest interfejsem, po którym dziedziczą klasy @a Game, @a Menu, @a Scores
 */
class GameState
{
public:
    GameState(Game* parent)
    {
        this->parent = parent;
    }

    virtual ~GameState() {}

    /**
     * Funkcja sprawdza czy aktualny stan gry się skończył.
     * @return @a true jeżeli obecny stan gry się skończył, w przeciwnym wypadku @a false.
     */
    virtual bool finished() const = 0;
    /**
     * Funkcja rysująca elementy gry należące do danego stanu
     */
//    virtual void draw() = 0;
    /**
     * Funkcja aktualizująca stan obiektów i liczników w grze
     * @param dt - czas od ostatniego wywołania funkcji w milisekundach
     */
    virtual void updateState(Uint32 dt) = 0;
    /**
     * Funkcja umożliwiająca obsługę zdarzeń wykrywanych przez bibliotekę SDL2.
     * @param ev - wskaźnik na unię SDL_Event przechowującą typ i parametry różnych zdarzeń
     */
    virtual void eventProcess(SDL_Event* ev) = 0;
    /**
     * Funkcja umożliwiająca obsługę zdarzeń przygotowanych na obecną ramkę.
     * @param events
     */
    virtual void eventProcess(EventsWrapper& events){}
    /**
     * Funkcja zwracającya następny stan po zakończeniu obecnego. Funkcję należy wywołać tylko wtedy, gdy funkcja @a finished zwróci wartość @a true.
     * @return następny stan gry
     */
    virtual GameState* nextState() = 0;

protected:
    bool m_finished;
    Game* parent;

    void setNetworkState(NetworkState state)
    {
        EnterCriticalSection(parent->critical_section);
            parent->shared_data->network_state = state;
        LeaveCriticalSection(parent->critical_section);
    }

    void saveEvent(Event* event)
    {
        parent->saveEvent(event);
    }
};
#endif // APPSTATE_H
