#ifndef SDL_SOUND_MANAGER_H
#define SDL_SOUND_MANAGER_H

#include "../sound_manager.h"

#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <vector>

namespace std
{
    template <class T>
    inline void hash_combine(std::size_t &s, const T &v)
    {
        std::hash<T> h;
        s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
    }

    template <>
    struct hash<Sound>
    {
        size_t operator()(const Sound &sound) const
        {
            return std::hash<std::string>()(sound.file_path);

            size_t seed = 0;
            hash_combine(seed, sound.file_path);
            hash_combine(seed, sound.volume);
            hash_combine(seed, sound.multi_instance);
            hash_combine(seed, sound.highest_priority);

            return seed;
        }
    };
}

class SDLSoundManager : public SoundManager
{
public:
    SDLSoundManager();
    ~SDLSoundManager() override;
    void preload(const Sound &sound) override;
    void play(const Sound &sound) override;
    void stop(const Sound &sound) override;
    void stopAll() override;

private:
    Mix_Chunk *loadSound(const Sound& sound);
    std::vector<Sound> findAllMatchingSounds(const Sound &sound);
 
    bool canPlay(const Sound &sound);

    std::unordered_map<Sound, Mix_Chunk *> m_sound_cache;
};

#endif // SDL_SOUND_MANAGER_H