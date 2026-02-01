#ifndef MENU_H
#define MENU_H

#include "appstate.h"
#include "../objects/tank/player.h"

#include <vector>
#include <string>

/**
 * @brief
 * A state for selecting the game mode: 1 player or 2 players mode or to exit.
 * This state is the first state of the application, appearing immediately after program startup and allowing transition to the game state.
 */
class Menu : public AppState
{
public:
    Menu(InteractiveComponents interactive_components, StateMachine* state_machine);
    ~Menu();
    /**
     * Function draws the game logo, menu texts, and the selected menu item indicator in the shape of a tank.
     */
    void draw(Renderer &renderer) override;

    /**
     * Function updates the tank pointer animation.
     * @param dt - time since the last function call, used for animation update
     */
    void update(const UpdateState& updateState) override;

    /**
     * Function responds to key presses:
     * @li Up and Down arrows - change the selected menu item
     * @li Enter and Space - confirm the current menu item
     * @li Esc - exit the application
     * @param event - reference to an Event object containing the type and parameters of various events
     */
    void eventProcess(const Event &event) override;

private:
    std::vector<std::string> m_menu_items;
    int m_current_menu_index;
    Player *m_tank_menu_pointer;

    int m_menu_item_height;
    Point m_first_menu_item_offset;
    Point m_tank_menu_pointer_offset;
};

#endif // MENU_H
