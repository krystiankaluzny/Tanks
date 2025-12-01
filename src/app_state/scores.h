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
    Scores(std::vector<Player*> players, int level, bool game_over);
    ~Scores() override = default;

    void draw(Renderer &renderer) override;
    void update(Uint32 dt) override;
    void eventProcess(const Event &event) override;

    AppState* nextState() override;

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

    bool m_finished;
};

#endif // SCORES_H
