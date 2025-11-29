#ifndef APP_H
#define APP_H

#include "engine/engine.h"
#include "app_state/appstate.h"

/**
 * @brief
 * Klasa główna aplikacji. Jej działanie jest równoważne z działaniem aplikacji.
 */
class App
{
public:
    App();
    ~App();
    /**
     * Funkcja zawiera inicjalizację biblioteki SDL2, silnika gry, wczytanie tekstur oraz czcionek.
     * Po poprawnej inicjalizacji następuje wejście w pętlę główną programu, która kolejno: reaguje na zdarzenie,
     * uaktualnia obecny stan aplikacji, rysuje obiety na ekranie.
     */
    void run();

private:
    /**
     * Zmienna podtrzymująca działanie pętli głównej programu.
     */
    bool is_running;
    /**
     * Aktualny stan aplikacji.
     */
    AppState *m_app_state;

    /**
    * Funkcja obsługująca zdarzenia czekające w kolejce.
    */
    void eventProces();

    ProcessingResult handleEvent(const Event &event);
    ProcessingResult updateState(Uint32 delta_time);
    ProcessingResult draw(Renderer &renderer);
};

#endif // APP_H
