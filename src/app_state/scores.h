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
    Scores(std::vector<Player*> players, int level);
    void draw();
    void update(Uint32 dt);
    void eventProcess(SDL_Event* ev);
    bool finished() const;
    AppState* nextState();

private:
    std::vector<Player*> m_players;
    Uint32 m_show_time;
    int m_level;
};

#endif // SCORES_H
