#ifndef EAGLE_H
#define EAGLE_H

#include "object.h"

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
    void update(Uint32 dt);
    void destroy();
};

#endif // EAGLE_H
