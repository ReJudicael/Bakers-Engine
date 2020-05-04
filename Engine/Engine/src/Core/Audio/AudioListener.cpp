#include "AudioListener.h"
#include "EngineCore.h"
#include "AudioSystem.h"
#include "RootObject.hpp"
RTTR_PLUGIN_REGISTRATION
{
    registration::class_<Core::Audio::AudioListener>("Audio Listener")
        .constructor()
        .property("Active", &Core::Audio::AudioListener::m_isActive);
}

namespace Core::Audio
{
    AudioListener::AudioListener() : ComponentUpdatable()
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
        ComponentUpdatable::OnCopy(copyTo);
        AudioListener* copy{ dynamic_cast<AudioListener*>(copyTo) };
        copy->m_isActive = m_isActive;
    }

    void AudioListener::OnReset()
    {
        ComponentUpdatable::OnReset();
    }

    void AudioListener::OnDestroy()
    {
        m_isActive = false;
    }

    void AudioListener::OnUpdate(float deltaTime)
    {
        // GetRoot()->GetEngine()->GetAudioSystem()->SetListenerTransform(GetParent()->GetTransform());
    }
}
