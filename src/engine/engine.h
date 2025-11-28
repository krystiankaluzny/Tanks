#ifndef ENGINE_H
#define ENGINE_H

#include "data/event.h"
#include "renderer.h"
#include "spriteconfig.h"
#include <functional>

enum class ProcessingResult
{
    STOP,
    CONTINUE
};

typedef std::function<ProcessingResult (Event&)> HandleEventFunc;
typedef std::function<ProcessingResult (int)> UpdateStateFunc;
typedef std::function<ProcessingResult (Renderer&)> DrawFunc;

/**
 * @brief Klasa łączy elementy związane z działaniem programu.
 */
class Engine
{
public:
    Engine();

    virtual void startMainLoop(HandleEventFunc handleEvent, UpdateStateFunc updateState, DrawFunc draw) = 0;

    /**
     * @return statyczną instancję obiektu @a Engine
     */
    static Engine& getEngine(); //silnik może być dostępny w każdym module korzystając jedynie z przestrzeni nazw a nie wskaźnika
    /**
     * Funkcja zamienia podaną wartość liczbową na tekst.
     * @param num - liczba całkowita
     * @return liczbę całkowitą jako tekst
     */
    static std::string intToString(int num);
    /**
     * Funkcja tworzy obiekty składowe silnika.
     */
    void initModules();
    /**
     * Funkcja niszczy obiekty składowe silnika.
     */
    void destroyModules();

    /**
     * @return wskaźnik na obiekt Renderer pozwalający rysować na ekranie
     */
    Renderer* getRenderer() const;
    /**
     * @return wsaźnik na obiekt SpriteConfig przechowujący informacjie o teksturach
     */
    SpriteConfig* getSpriteConfig() const;
private:
    Renderer* m_renderer;
    SpriteConfig* m_sprite_config;
};

#endif // ENGINE_H
