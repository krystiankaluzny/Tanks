#ifndef SCORES_H
#define SCORES_H
#include "appstate.h"
#include "../objects/player.h"

#include <vector>
#include <string>

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
    std::vector<Player*> m_players;
    int m_level;
    bool m_game_over;
    unsigned m_score_counter;
    bool m_score_counter_run;
    unsigned m_max_score;
    Uint32 m_show_time;
    bool m_finished;
};

#endif // SCORES_H
