#ifndef EAGLE_H
#define EAGLE_H

#include "object.h"


class Eagle : public Object
{
public:
    Eagle(double x, double y);

    void update(Uint32 dt) override;
    void destroy();
};

#endif // EAGLE_H
