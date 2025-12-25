#include "sdl_sound_manager.h"
#include <iostream>

SDLSoundManager::SDLSoundManager()
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

SDLSoundManager::~SDLSoundManager()
{
    for (auto &pair : m_sound_cache)
    {
        Mix_FreeChunk(pair.second);
    }
    m_sound_cache.clear();
    Mix_CloseAudio();
}

Mix_Chunk *SDLSoundManager::loadSound(const std::string &file_path)
{
    auto it = m_sound_cache.find(file_path);
    if (it != m_sound_cache.end())
    {
        return it->second;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(file_path.c_str());
    if (chunk == nullptr)
    {
        std::cerr << "Failed to load sound: " << file_path << " SDL_mixer Error: " << Mix_GetError() << std::endl;
        return nullptr;
    }

    m_sound_cache[file_path] = chunk;
    return chunk;
}

void SDLSoundManager::play(const Sound &sound)
{
    Mix_Chunk *chunk = loadSound(sound.file_path);
    if (chunk == nullptr)
        return;

    int loops = sound.loop ? -1 : 0;
    int channel = Mix_PlayChannel(-1, chunk, loops);
    if (channel == -1)
    {
        std::cerr << "Failed to play sound: " << sound.file_path << " SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    int volume = static_cast<int>(MIX_MAX_VOLUME * (sound.volume / 100.0));
    Mix_Volume(channel, volume);
}

void SDLSoundManager::stop(const Sound &sound)
{
    Mix_Chunk *chunk = loadSound(sound.file_path);
    if (chunk == nullptr)
        return;

    // Halt all channels playing this chunk
    for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
    {
        if (Mix_GetChunk(i) == chunk)
        {
            Mix_HaltChannel(i);
        }
    }
}
