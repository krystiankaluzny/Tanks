#ifndef ENGINE_INTERACTIVE_COMPONENTS_H
#define ENGINE_INTERACTIVE_COMPONENTS_H

#include "sound_manager.h"

struct InteractiveComponents
{
    SoundManager *sound_manager;
};

class InteractiveComponentsHolder
{
public:
    InteractiveComponentsHolder(InteractiveComponents interactive_components)
        : m_interactive_components(interactive_components) {}

    virtual ~InteractiveComponentsHolder() = default;

protected:
    void preloadSound(const Sound &sound)
    {
        m_interactive_components.sound_manager->preload(sound);
    }
    void playSound(const Sound &sound)
    {
        m_interactive_components.sound_manager->play(sound);
    }
    void stopSound(const Sound &sound)
    {
        m_interactive_components.sound_manager->stop(sound);
    }
    void stopAllSounds()
    {
        m_interactive_components.sound_manager->stopAll();
    }

    InteractiveComponents m_interactive_components;
};

#endif // ENGINE_INTERACTIVE_COMPONENTS_H