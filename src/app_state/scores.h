#ifndef SCORES_H
#define SCORES_H
#include "appstate.h"
#include "../objects/player.h"

#include <vector>
#include <string>

class Scores : public AppState
{
public:
    Scores();
    Scores(std::vector<Player*> players, int level, bool game_over);
    void draw();
    void update(Uint32 dt);
    void eventProcess(SDL_Event* ev);
    bool finished() const;
    AppState* nextState();

private:
    std::vector<Player*> m_players;
    int m_level;
    bool m_game_over;
    int m_score_counter;
    bool m_score_counter_run;
    int m_max_score;
    Uint32 m_show_time;
};

#endif // SCORES_H
