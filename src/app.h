#ifndef APP_H
#define APP_H

#define VERSION "1.0.1"

/**
 * @brief
 * Klasa główna aplikacji. Jej działanie jest równoważne z działaniem aplikacji.
 */
class App
{
public:
    App();
    void run();

private:
    bool initSDL();
    void quitSDL();
};

#endif // APP_H
