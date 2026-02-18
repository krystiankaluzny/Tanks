#ifndef SCORES_H
#define SCORES_H
#include "appstate.h"
#include "../objects/tank/player/player.h"
#include "../engine/state_machine/context_state.h"
#include <vector>
#include <string>

class Scores : public AppState
{
public:
    Scores(std::vector<Player *> players, int level, bool game_over, InteractiveComponents interactive_components, StateMachine *parent_state_machine);
    ~Scores() override;

    void draw(Renderer &renderer) override;
    void update(const UpdateState &updateState) override;
    void eventProcess(const Event &event) override;

protected:
    void onInitialize() override;

private:
    void transiteToNextState();

    StateMachine* m_scores_state_machine;

    std::vector<Player *> m_players;
    int m_level;
    bool m_game_over;
    unsigned m_score_counter;
    unsigned m_max_score;

    // Sub-states for score counting and idle state after counting
    class CountingState : public ContextState<Scores>
    {
    public:
        CountingState(Scores *ps);
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;

    private:
        Uint32 m_single_score_count_time;
    };

    class IdleState : public ContextState<Scores>
    {
    public:
        IdleState(Scores *ps);
        void update(const UpdateState &updateState) override;
        void eventProcess(const Event &event) override;

    private:
        Uint32 m_idle_time;
    };
};

#endif // SCORES_H
