#ifndef BULLET_H
#define BULLET_H

#include "object.h"

/**
 * @brief Klasa zajmująca się pociskami wystrzalanymi przez czołgi.
 */
class Bullet : public Object
{
public:
    /**
     * Tworzenie pocisku w położeniu (0, 0).
     */
    Bullet();
    /**
     * Tworzenie pocisku
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     */
    Bullet(double x, double y);

    /**
     * Aktualizacja położenia pocisku.
     * @param dt - czas od ostatniego wywołania funkcji
     */
    void update(Uint32 dt);
    /**
     * Włączenie animacji wybuchu pocisku.
     */
    void destroy();
    
    /**
     * Prędkość poruszania się pocisku.
     */
    double speed;
    /**
     * Zmienna przechowuje informację czy pocisk się z czymś zderzył.
     */
    bool collide;
    /**
     * Zmienna przechowuje informację czy pocisk ma zwiększone obrażenia.
     * Zwiększone obrażenia pozwalają niszczyć kamienny mur oraz krzaki.
     */
    bool increased_damage;
    /**
     * Kierunek przemieszczania się pocisku.
     */
    Direction direction;
};

#endif // BULLET_H
