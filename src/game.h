#ifndef GAME_H
#define GAME_H

#include "game_state/gamestate.h"

/**
 * @brief
 * Klasa główna aplikacji. Jej działanie jest równoważne z działaniem aplikacji.
 */
class Game
{
public:
    Game();
    ~Game();
    /**
     * Funkcja zawiera inicjalizację biblioteki SDL2, silnika gry, wczytanie tekstur oraz czcionek.
     * Po poprawnej inicjalizacji następuje wejście w pętlę główną programu, która kolejno: reaguje na zdarzenie,
     * uaktualnia obecny stan aplikacji, rysuje obiety na ekranie.
     */
    void run();
    /**
     * Funkcja obsługująca zdarzenia czekające w kolejce.
     */
    void eventProces();
private:
    /**
     * Zmienna podtrzymująca działanie pętli głównej programu.
     */
    bool is_running;
    /**
     * Aktualny stan aplikacji.
     */
    GameState* m_game_state;
    /**
     * Obiekt okna aplikacji.
     */
    SDL_Window* m_window;
};

#endif // GAME_H
