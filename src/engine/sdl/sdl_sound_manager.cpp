#include "sdl_sound_manager.h"
#include "../data/error.h"
#include <iostream>

#define MIX_CHANNEL_MONO 1
#define MIX_CHANNEL_STEREO 2
#define MIX_CHUNK_SIZE 2048

std::unordered_map<int, std::pair<Sound, Mix_Chunk *>> m_current_channel_chunk_cache;

void onChannelFinished(int channel)
{
    m_current_channel_chunk_cache[channel] = std::make_pair(Sound{}, nullptr);
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

    auto matching_sounds = findAllMatchingSounds(sound);

    Sound sound_to_load;

    if (matching_sounds.empty())
    {
        sound_to_load = sound;
    }
    else
    {
        // Find the lowest volume among matching sounds to play a quieter version to prevent clipping
        double min_volume = 100.0;
        for (const auto &matching_sound : matching_sounds)
        {
            if (matching_sound.volume < min_volume)
            {
                min_volume = matching_sound.volume;
            }
        }

        sound_to_load = Sound{sound.file_path, min_volume * sound.overlaping_factor, sound.overlaping_factor, sound.multi_instance, sound.highest_priority};
    }

    Mix_Chunk *chunk = loadSound(sound_to_load);

    if (chunk == nullptr)
        return;

    int loops = 0;
    int channel = Mix_PlayChannel(-1, chunk, loops);
    if (channel == -1)
    {
        std::cerr << "Warning: Unable to play sound: " << sound_to_load.file_path << " Error: " << Mix_GetError() << std::endl;
    }

    m_current_channel_chunk_cache[channel] = std::make_pair(sound_to_load, chunk);
}

void SDLSoundManager::stop(const Sound &sound)
{
    for (const auto &channel_to_sound : m_current_channel_chunk_cache)
    {
        auto current_sound = channel_to_sound.second.first;
        if (current_sound.file_path == sound.file_path)
        {
            //BE AWARE: Mix_ChannelFinished callback will be called after this and can modify m_current_channel_chunk_cache
            Mix_HaltChannel(channel_to_sound.first);
        }
    }
}

void SDLSoundManager::stopAll()
{
    Mix_HaltChannel(-1);
    m_current_channel_chunk_cache.clear();
}

Mix_Chunk *SDLSoundManager::loadSound(const Sound &sound)
{
    const std::string &file_path = sound.file_path;
    auto it = m_sound_cache.find(sound);
    if (it != m_sound_cache.end())
    {
        return it->second;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(file_path.c_str());
    if (chunk == nullptr)
    {
        throw Error("Failed to load sound: " + file_path, Mix_GetError());
    }

    int volume = static_cast<int>(MIX_MAX_VOLUME * (sound.volume / 100.0));
    Mix_VolumeChunk(chunk, volume);

    m_sound_cache[sound] = chunk;
    return chunk;
}

std::vector<Sound> SDLSoundManager::findAllMatchingSounds(const Sound &sound)
{
    std::vector<Sound> matching_sounds;
    for (const auto &channel_to_sound : m_current_channel_chunk_cache)
    {
        auto current_sound = channel_to_sound.second.first;
        if (sound.file_path == current_sound.file_path)
        {
            matching_sounds.push_back(current_sound);
        }
    }
    return matching_sounds;
}

bool SDLSoundManager::canPlay(const Sound &sound)
{
    if (!sound.multi_instance)
    {
        // Check if this Sound is already playing
        for (const auto &channel_to_sound : m_current_channel_chunk_cache)
        {
            auto current_sound = channel_to_sound.second.first;
            if (sound.file_path == current_sound.file_path)
            {
                return false;
            }
        }
    }

    if (!sound.highest_priority)
    {
        // If there is a highest priority sound playing, do not play this sound
        for (const auto &channel_to_sound : m_current_channel_chunk_cache)
        {
            auto current_sound = channel_to_sound.second.first;
            if (current_sound.highest_priority)
            {
                return false; // Do not play this sound
            }
        }
    }
    return true;
}