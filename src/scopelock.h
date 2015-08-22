#ifndef SCOPELOCK_H
#define SCOPELOCK_H
#include <SDL2/SDL_mutex.h>

class SdlMutex
{
public:
    SdlMutex();

    ~SdlMutex();

    void lock();

    void unlock();

    SDL_mutex* underlying();
private:
    SDL_mutex* mutex;
};

class SdlScopedLock
{
public:

    SdlScopedLock( SdlMutex& mutex );
    ~SdlScopedLock();
    void unlock();

private:
    SdlMutex& mutex;
};

#endif // SCOPELOCK_H
