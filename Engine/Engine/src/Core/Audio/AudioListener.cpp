#include "AudioListener.h"

RTTR_PLUGIN_REGISTRATION
{
    registration::class_<Core::Audio::AudioListener>("Audio Listener")
        .constructor()
        .property("Active", &Core::Audio::AudioListener::m_isActive);
}

namespace Core::Audio
{
    AudioListener::AudioListener() : ComponentBase()
    {
    }

    AudioListener::~AudioListener()
    {
    }

    void AudioListener::StartCopy(IComponent*& copyTo) const
    {
        copyTo = new AudioListener();
        OnCopy(copyTo);
    }

    void AudioListener::OnCopy(IComponent* copyTo) const
    {
        ComponentBase::OnCopy(copyTo);
        AudioListener* copy{ dynamic_cast<AudioListener*>(copyTo) };
        copy->m_isActive = m_isActive;
    }

    void AudioListener::OnReset()
    {
        ComponentBase::OnReset();
    }

    void AudioListener::OnDestroy()
    {
        m_isActive = false;
    }
}
