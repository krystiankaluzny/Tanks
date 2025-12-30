#include "sdl_sound_manager.h"
#include "../data/error.h"
#include <iostream>

#define MIX_CHANNEL_MONO 1
#define MIX_CHANNEL_STEREO 2
#define MIX_CHUNK_SIZE 2048

std::unordered_map<int, std::pair<Sound, Mix_Chunk *>> m_current_channel_chunk_cache;

void onChannelFinished(int channel)
{
    m_current_channel_chunk_cache.erase(channel);
}

SDLSoundManager::SDLSoundManager()
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNEL_STEREO, MIX_CHUNK_SIZE) < 0)
    {
        throw Error("SDL_mixer could not initialize!", Mix_GetError());
    }

    Mix_ChannelFinished(onChannelFinished);
}

SDLSoundManager::~SDLSoundManager()
{
    m_current_channel_chunk_cache.clear();

    for (auto &pair : m_sound_cache)
    {
        Mix_FreeChunk(pair.second);
    }
    m_sound_cache.clear();
    Mix_CloseAudio();
}

void SDLSoundManager::play(const Sound &sound)
{
    Mix_Chunk *chunk = loadSound(sound.file_path);
    if (chunk == nullptr)
        return;

    if (!canPlay(sound))
    {
        return;
    }

    if (sound.highest_priority)
    {
        // Stop all currently playing sounds
        Mix_HaltChannel(-1);
        m_current_channel_chunk_cache.clear();
    }

    int volume = static_cast<int>(MIX_MAX_VOLUME * (sound.volume / 100.0));
    Mix_VolumeChunk(chunk, volume);

    int loops = 0;
    int channel = Mix_PlayChannel(-1, chunk, loops);
    if (channel == -1)
    {
        std::cerr << "Warning: Unable to play sound: " << sound.file_path << " Error: " << Mix_GetError() << std::endl;
    }

    m_current_channel_chunk_cache[channel] = std::make_pair(sound, chunk);
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

void SDLSoundManager::stopAll()
{
    Mix_HaltChannel(-1);
    m_current_channel_chunk_cache.clear();
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
        throw Error("Failed to load sound: " + file_path, Mix_GetError());
    }

    m_sound_cache[file_path] = chunk;
    return chunk;
}

bool SDLSoundManager::canPlay(const Sound &sound)
{
    if (!sound.multi_instance)
    {
        // Check if this Sound is already playing
        for (const auto &pair : m_current_channel_chunk_cache)
        {
            auto playing_sound = pair.second.first;
            if (sound == playing_sound)
            {
                return false;
            }
        }
    }

    if (!sound.highest_priority)
    {
        // If there is a highest priority sound playing, do not play this sound
        for (const auto &pair : m_current_channel_chunk_cache)
        {
            auto playing_sound = pair.second.first;
            if (playing_sound.highest_priority)
            {
                return false; // Do not play this sound
            }
        }
    }
    return true;
}