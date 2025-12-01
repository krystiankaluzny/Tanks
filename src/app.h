#ifndef APP_H
#define APP_H

#include "engine/engine.h"
#include "app_state/appstate.h"

class App
{
public:
    App();
    ~App();

    void run();

private:
    AppState *m_app_state;

    ProcessingResult handleEvent(const Event &event);
    ProcessingResult updateState(Uint32 delta_time);
    ProcessingResult draw(const Renderer &renderer);
};

#endif // APP_H
