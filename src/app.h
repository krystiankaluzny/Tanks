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

    ProcessingResult onEngineInit(const Engine& engine);
    ProcessingResult handleEvent(const Event &event);
    ProcessingResult updateState(const UpdateState& updateState);
    ProcessingResult draw(Renderer &renderer);
};

#endif // APP_H
