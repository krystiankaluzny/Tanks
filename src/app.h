#ifndef APP_H
#define APP_H

#include "app_state/appstate.h"

class App
{
public:
    App();
    ~App();
    void run();
    void eventProces();
private:
    bool is_running;
    AppState* m_app_state;
};

#endif // APP_H
