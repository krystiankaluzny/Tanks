#ifndef ENGINE_SOUND_MANAGER_H
#define ENGINE_SOUND_MANAGER_H

#include <string>

struct Sound
{
    std::string file_path;
    double volume; // 0 - 100
    bool loop;
};

class SoundManager
{
private:
    /* data */
public:
    SoundManager() = default;
    virtual ~SoundManager() = default;

    virtual void play(const Sound &sound) = 0;
    virtual void stop(const Sound &sound) = 0;
};

#endif // ENGINE_SOUND_MANAGER_H