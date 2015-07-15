#ifndef EAGLE_H
#define EAGLE_H

#include "object.h"

/**
 * @brief Klasa orzełka, którego gracze mają bronić a przeciwnicy zniszczyć.
 */
class Eagle : public Object
{
public:
    /**
     * Tworzenie orzełka w położeniu (0, 0).
     */
    Eagle();
    /**
     * Tworzenie orzełka
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     */
    Eagle(double x, double y);
    /**
     * Aktualizacja animacji orzełka.
     * @param dt - czas od ostatniego wywołania funkcji
     */
    void update(Uint32 dt);
    /**
     * Wywołanie wybuchu orzełka.
     */
    void destroy();
};

#endif // EAGLE_H
