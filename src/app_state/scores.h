#ifndef SCORES_H
#define SCORES_H
#include "appstate.h"
#include "../objects/player.h"

#include <vector>
#include <string>

/**
 * @brief
 * Klasa odpowada za wyświetlanie wyników graczy z poprzedniej rundy.
 */
class Scores : public AppState
{
public:
    Scores();
    /**
     * Konstruktor wywoływany przez Game po zakończeniu rozgrywki.
     * @param players - kontener z wszystkimi graczami, którzy brali udział w rozgrywce
     * @param level - numer ostatniego poziomu
     * @param game_over - zmianan mówiąca czy ostatni poziom został przegrany
     */
    Scores(std::vector<Player*> players, int level, bool game_over);
    /**
     * Funkcja zwraca @a true po określonym czasie wyświetlania ekranu punktów.
     * @return @a true lub @a false
     */
    bool finished() const;
    /**
     * Funkcja odpowiada za rysowanie odpowiednich tekstów i czołgów graczy na ekranie.
     */
    void draw();
    /**
     * Funkcaj odpowiada za zmianę licznika punktów.
     * @param dt - czas od ostatniego wywołania funkcji pozwala określić czas wyświetlania okna wyników
     */
    void update(Uint32 dt);
    /**
     * Funkcja odpowiada na wciśnięcie klawisza Enter, co kończy odliczanie punktów i przyspiesza przejście do następnego stanu.
     * @param ev - wskaźnik na unię SDL_Event przechowującą typ i parametry różnych zdarzeń
     */
    void eventProcess(SDL_Event* ev);
    /**
     * Funkcja zwraca wskaźnik na obiekt będący następnym stanem aplikacji. Jeśli gracz przegrał następnym stanem jest @a Menu jeśli przeszedł rundę następnym stanem jest @a Game.
     * @return wskaźnik na następny stan
     */
    AppState* nextState();

private:
    /**
     * Kontener z wszystkimi graczami (zabitymi i niezabitymi).
     */
    std::vector<Player*> m_players;
    /**
     * Numer ostatniego poziomu.
     */
    int m_level;
    /**
     * Zmianna przechowuje czy ostatnie poziom był przegrany.
     */
    bool m_game_over;
    /**
     * Aktualna wartości licznika punktów.
     */
    unsigned m_score_counter;
    /**
     * Zmianna mówiąca czy licznik punktów jest włączony.
     */
    bool m_score_counter_run;
    /**
     * Wynik do jakiego dąży licznik punktów.
     */
    unsigned m_max_score;
    /**
     * Czas od zakończenia zliczania punktów w milisekundach.
     */
    Uint32 m_show_time;
};

#endif // SCORES_H
