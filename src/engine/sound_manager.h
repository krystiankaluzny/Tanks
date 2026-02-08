#ifndef ENGINE_SOUND_MANAGER_H
#define ENGINE_SOUND_MANAGER_H

#include <string>

struct Sound
{
    std::string file_path;
    double volume;                  // 0 - 100
    double overlapping_factor = 1.0; // factor to reduce volume if multiple instances are played simultaneously
    bool multi_instance;            // if true - it is allowed to play multiple instances of the sound simultaneously
    bool highest_priority = false;  // if true - this sound has highest priority when played (stops other sounds if needed)

    inline bool operator==(const Sound &rhs) const
    {
        return file_path == rhs.file_path &&
               volume == rhs.volume &&
               multi_instance == rhs.multi_instance &&
               highest_priority == rhs.highest_priority;
    }
};

class SoundManager
{
private:
    /* data */
public:
    SoundManager() = default;
    virtual ~SoundManager() = default;

    virtual void preload(const Sound &sound) = 0;
    virtual void play(const Sound &sound) = 0;
    virtual void stop(const Sound &sound) = 0;
    virtual void stopAll() = 0;
};

#endif // ENGINE_SOUND_MANAGER_H