#include "appstate.h"
#include <iostream>

AppState::AppState(InteractiveComponents m_interactive_components) : m_interactive_components(m_interactive_components)
{
}

void AppState::playSound(const Sound &sound)
{
    std::cout << "Playing sound: " << sound.file_path << std::endl;
    m_interactive_components.sound_manager->play(sound);
}

void AppState::stopAllSounds()
{
    m_interactive_components.sound_manager->stopAll();
}