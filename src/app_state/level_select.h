#ifndef LEVEL_SELECT_H
#define LEVEL_SELECT_H

#include "appstate.h"

class LevelSelect : public AppState
{
public:
    LevelSelect(int players_count, InteractiveComponents interactive_components, StateMachine *state_machine);

    void draw(Renderer &renderer) override;
    void update(const UpdateState &updateState) override;
    void eventProcess(const Event &event) override;

protected:
    void onInitialize() override;

private:
    int m_players_count;
    int m_selected_level;
};

#endif // LEVEL_SELECT_H
