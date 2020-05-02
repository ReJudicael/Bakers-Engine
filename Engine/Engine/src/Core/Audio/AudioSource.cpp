#include "AudioSource.h"

RTTR_PLUGIN_REGISTRATION
{
    registration::class_<Core::Audio::AudioSource>("Audio Source")
        .constructor()
        .property("Audio Clip", &Core::Audio::AudioSource::m_audioClip)
        .property("Mute", &Core::Audio::AudioSource::m_mute)
        .property("Play On Awake", &Core::Audio::AudioSource::m_playOnAwake)
        .property("Loop", &Core::Audio::AudioSource::m_loop)
        .property("Priority", &Core::Audio::AudioSource::m_priority)
        .property("Volume", &Core::Audio::AudioSource::m_volume)
        .property("Pitch", &Core::Audio::AudioSource::m_pitch)
        .property("Stereo Pan", &Core::Audio::AudioSource::m_stereoPan);
}

namespace Core::Audio
{
    AudioSource::AudioSource() : ComponentBase()
    {
    }

    AudioSource::~AudioSource()
    {
    }

    void AudioSource::StartCopy(IComponent*& copyTo) const
    {
        copyTo = new AudioSource();
        OnCopy(copyTo);
    }

    void AudioSource::OnCopy(IComponent* copyTo) const
    {
        ComponentBase::OnCopy(copyTo);
        AudioSource* copy{ dynamic_cast<AudioSource*>(copyTo) };
        copy->m_isActive = m_isActive;
    }

    void AudioSource::OnReset()
    {
        ComponentBase::OnReset();
    }

    void AudioSource::OnDestroy()
    {
        m_isActive = false;
    }
}
