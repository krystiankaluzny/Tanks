#ifndef BRICK_H
#define BRICK_H

#include "object.h"

/**
 * @brief Klasa odpowiedzialna za pojedyńczy kawałek murku.
 */
class Brick : public Object
{
public:
    /**
     * Tworzenie murku w położeniu (0, 0).
     */
    Brick();
    /**
     * Tworzenie murku
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     */
    Brick(double x, double y);

    /**
     * Funkcja została zaimplementowana aby nie zmieniać collision_rect.
     * @param dt - czas od ostatniego wywołania funkcji.
     */
    void update(Uint32 dt);
    /**
     * Funkcja zmieniająca stan murku po trafieniu przez pocisk. Funkcja zmienia collision_rect.
     * @param bullet_direction - kierunek ruchu pocisku
     */
    void bulletHit(Direction bullet_direction);
private:
    /**
     * Liczba trafień pucisków w murek.
     */
    int m_collision_count;
    /**
     * Jeden z dziesięciu stanów w jakich może być murek.
     */
    int m_state_code;
};

#endif // BRICK_H
