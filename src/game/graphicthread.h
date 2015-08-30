#ifndef GRAPHICTHREAD_H
#define GRAPHICTHREAD_H

#include <SDL2/SDL.h>
#include "../appthread.h"

class GraphicThread : public AppThread
{
public:
    GraphicThread(SharedData *shared_data, CRITICAL_SECTION *critical_section);

    void run();
private:
    void mainLoop();

    bool initSDL();

    void quitSDL();

    void eventProcess();

    bool is_running;


    /**
     * Obiekt okna aplikacji.
     */
    SDL_Window* m_window;


};

#endif // GRAPHICTHREAD_H
