#include "scopelock.h"


SdlMutex::SdlMutex()
{
    mutex = SDL_CreateMutex();
//    if ( !mutex ) throw std::runtime_error( "SDL_CreateMutex == NULL" );
}

SdlMutex::~SdlMutex()
{
    SDL_DestroyMutex( mutex );
}

void SdlMutex::lock()
{
//    if( SDL_mutexP( mutex ) == -1 )
//        throw std::runtime_error( "SDL_mutexP == -1" );
    //  Note:
    //      -1 does not mean it was already locked - it means there was an error in locking -
    //      if it was locked it will just block - see SDL_mutexP(3)
}

void SdlMutex::unlock()
{
//    if ( SDL_mutexV( mutex ) == -1 )
//        throw std::runtime_error( "SDL_mutexV == -1" );
}

SDL_mutex *SdlMutex::underlying()
{
    return mutex;
}


SdlScopedLock::SdlScopedLock(SdlMutex &mutex) :
    mutex(mutex)
{
    mutex.lock();
}

SdlScopedLock::~SdlScopedLock()
{
//    try
//    {
//        this->unlock();
//    }
//    catch( const std::exception& e )
//    {
//        // Destructors should never throw ...
//        std::cerr << "SdlScopedLock::~SdlScopedLock - caught : " << e.what() << std::endl;
//    }
}

void SdlScopedLock::unlock()
{
    mutex.unlock();
}
