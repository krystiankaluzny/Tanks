#ifndef ENGINE_H
#define ENGINE_H

#include "renderer.h"
#include "spriteconfig.h"

//klasa zawiera wymagane alementy do poprawnego działania mechaniki gry
class Engine
{
public:
    Engine();
    static Engine& getEngine(); //silnik może być dostępny w każdym module korzystając jedynie z przestrzeni nazw a nie wskaźnika

    void initModules(); //tworzenie obiektów składowych silnika
    void destroyModules(); //niszczenie obiektów składowych silnika

    //pobieranie elementu składowych silnika
    Renderer* getRenderer() const;
    SpriteConfig* getSpriteConfig() const;
private:
    Renderer* m_renderer;
    SpriteConfig* m_sprite_config;
};

#endif // ENGINE_H
