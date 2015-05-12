#ifndef MENU_H
#define MENU_H

#include "appstate.h"
#include "../objects/player.h"

#include <vector>
#include <string>

class Menu : public AppState
{
public:
    Menu();
    ~Menu();
    bool finished() const;
    void draw();
    void update(Uint32 dt);
    void eventProcess(SDL_Event* ev);
    AppState* nextState();

private:
    std::vector<std::string> m_menu_texts;
    int m_menu_index;
    Player* m_tank_pointer;
    bool m_finished;
};

#endif // MENU_H
