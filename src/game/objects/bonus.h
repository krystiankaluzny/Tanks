#ifndef BONUS_H
#define BONUS_H

#include "object.h"

/**
 * @brief Klasa zajumująca wyświetlaniem bonusu.
 */
class Bonus : public Object
{
public:
    /**
     * Tworzenie domyślnego bonusu - gwiazdki w położeniu (0, 0).
     */
    Bonus();
    /**
     * Tworzenie bonusu.
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ bonusu
     */
    Bonus(double x, double y, SpriteType type);

    /**
     * Funkcja rysująca bonus.
     */
    void draw();
    /**
     * Funkcja uaktualniająca animacje bonusu. Pozwolenie na usunięcie po odpowiednim czasie.
     * Zwiększenie częstotliwości migania jeżeli bonus ma niedługo zostać usunięty.
     * @param dt - czas od ostatniego wywołania funkcji
     */
    void update(Uint32 dt);
private:
    /**
     * Czas od stworzenie bonusu.
     */
    Uint32 m_bonus_show_time;
    /**
     * Zmienna przechowująca informacje o tym czy bonus jest aktualnie wyświetlany; wykorzystywana do migania.
     */
    bool m_show;
};

#endif // BONUS_H
