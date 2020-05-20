#include "AudioListener.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "AudioSystem.h"
#include <fmod.hpp>

RTTR_PLUGIN_REGISTRATION
{
    ZoneScopedN("Registering RTTR")
    registration::class_<Core::Audio::AudioListener>("Audio Listener")
        .constructor()
        .property("Active", &Core::Audio::AudioListener::m_isActive);
}

namespace Core::Audio
{
    AudioListener::AudioListener() : ComponentUpdatable()
    {
    }

    void AudioListener::StartCopy(IComponent*& copyTo) const
    {
        ZoneScoped
        copyTo = new AudioListener();
        OnCopy(copyTo);
    }

    bool AudioListener::OnStart()
    {
        ZoneScoped
        return ComponentUpdatable::OnStart();
    }

    void AudioListener::OnCopy(IComponent* copyTo) const
    {
        ZoneScoped
        ComponentUpdatable::OnCopy(copyTo);
        AudioListener* copy{ dynamic_cast<AudioListener*>(copyTo) };
        copy->m_isActive = m_isActive;
    }

    void AudioListener::OnReset()
    {
        ComponentUpdatable::OnReset();
        m_isActive = true;
    }

    void AudioListener::OnDestroy()
    {
        ComponentUpdatable::OnDestroy();
        m_isActive = false;
    }

    void AudioListener::OnUpdate(float deltaTime)
    {
        ComponentUpdatable::OnUpdate(deltaTime);

        Core::Datastructure::Object* parent = GetParent();
        Core::Maths::Vec3 pos = parent->GetGlobalPos();
        Core::Maths::Quat rot = parent->GetGlobalRot();

        // TODO: (pos-lastpos) / time_taken_since_last_frame_in_seconds.
        Core::Maths::Vec3 vel = { 0.f, 0.f, 0.f };

        Core::Maths::Vec3 forward = rot * Maths::Vec3(0.0f, 0.0f, 1.0f);
        Core::Maths::Vec3 up = rot * Maths::Vec3(0.0f, 1.0f, 0.0f);

        // Set 3D attributes
        FMOD_RESULT result = GetRoot()->GetEngine()->GetAudioSystem()->GetFMODSystem()->set3DListenerAttributes(
            0,
            reinterpret_cast<FMOD_VECTOR*>(&pos),
            reinterpret_cast<FMOD_VECTOR*>(&vel),
            reinterpret_cast<FMOD_VECTOR*>(&forward),
            reinterpret_cast<FMOD_VECTOR*>(&up));

        CHECK_ERR_FMOD(result);
    }
}
