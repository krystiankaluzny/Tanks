#ifndef HANDLER_H
#define HANDLER_H

#include "../../event/eventstore.h"
#include "../../appthread.h"

class Handler
{
public:
    Handler();
    void exec(EventsWrapper& events);
};

#endif // HANDLER_H
