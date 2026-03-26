#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "appstate.h"

class StageSelect : public AppState
{
public:
    StageSelect(int players_count, InteractiveComponents interactive_components, StateMachine *state_machine);

    void draw(Renderer &renderer) override;
    void update(const UpdateState &updateState) override;
    void eventProcess(const Event &event) override;

protected:
    void onInitialize() override;

private:
    int m_players_count;
    int m_selected_stage;
};

#endif // STAGE_SELECT_H
