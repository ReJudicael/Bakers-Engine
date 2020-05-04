#include <fmod.hpp>
#include <fmod_errors.h>

#include "Object.hpp"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "AudioSystem.h"
#include "AudioSource.h"

RTTR_PLUGIN_REGISTRATION
{
    using namespace Core::Datastructure;

    registration::class_<Core::Audio::AudioSource>("Audio Source")
        .enumeration<Core::Audio::EAudioMode>("Audio Mode")
        (
            value("2D", Core::Audio::EAudioMode::AUDIO_2D),
            value("3D", Core::Audio::EAudioMode::AUDIO_3D)
        )

        .constructor()
        .property("Audio Clip",     &Core::Audio::AudioSource::m_audioClip)
        .property("Audio Mode",     &Core::Audio::AudioSource::m_audioMode)
        .property("Mute",           &Core::Audio::AudioSource::m_mute)
        .property("Play On Awake",  &Core::Audio::AudioSource::m_playOnAwake)
        .property("Loop",           &Core::Audio::AudioSource::m_loop)
        .property("Priority",       &Core::Audio::AudioSource::m_priority)
        .property("Volume",         &Core::Audio::AudioSource::m_volume)
        .property("Pitch",          &Core::Audio::AudioSource::m_pitch)
        .property("Pan",            &Core::Audio::AudioSource::m_pan)
        .property("Min Distance",   &Core::Audio::AudioSource::m_minDistance)
        .property("Max Distance",   &Core::Audio::AudioSource::m_maxDistance);
}

namespace Core::Audio
{
    AudioSource::AudioSource() : ComponentUpdatable()
    {
    }

    void AudioSource::StartCopy(IComponent*& copyTo) const
    {
        copyTo = new AudioSource();
        OnCopy(copyTo);
    }

    void AudioSource::OnCopy(IComponent* copyTo) const
    {
        ComponentUpdatable::OnCopy(copyTo);
        AudioSource* copy{ dynamic_cast<AudioSource*>(copyTo) };

        copy->m_audioClip       = m_audioClip;
        copy->m_audioMode       = m_audioMode;
        copy->m_mute            = m_mute;
        copy->m_playOnAwake     = m_playOnAwake;
        copy->m_loop            = m_loop;
        copy->m_priority        = m_priority;
        copy->m_volume          = m_volume;
        copy->m_pitch           = m_pitch;
        copy->m_pan             = m_pan;
        copy->m_minDistance     = m_minDistance;
        copy->m_maxDistance     = m_maxDistance;
    }

    void AudioSource::OnReset()
    {
        ComponentUpdatable::OnReset();

        m_audioClip     = "";
        m_audioMode     = Core::Audio::EAudioMode::AUDIO_2D;
        m_mute          = false;
        m_playOnAwake   = true;
        m_loop          = false;
        m_priority      = 128;
        m_volume        = 1.f;
        m_pitch         = 1.f;
        m_pan           = 0.f;
        m_minDistance   = 1.f;
        m_maxDistance   = 100.f;
    }

    void AudioSource::OnDestroy()
    {
        ComponentUpdatable::OnDestroy();

        Stop();
        if (!m_fmodSound)
            return;

        FMOD_RESULT result = m_fmodSound->release();
        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }

    void AudioSource::OnStart()
    {
        ComponentUpdatable::OnStart();

        if (m_audioClip.empty())
            return;

        CreateSound(m_audioClip);

        if (m_playOnAwake)
            Play();
    }

    void AudioSource::Play()
    {
        if (IsPlaying())
            return;

        m_fmodSound->setMode(static_cast<FMOD_MODE>(m_audioMode) | (m_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
        FMOD_RESULT result = GetRoot()->GetEngine()->GetAudioSystem()->GetFMODSystem()->playSound(m_fmodSound, nullptr, false, &m_fmodChannel);
        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }

    void AudioSource::Pause()
    {
        if (!IsPlaying())
            return;

        bool isPaused;
        FMOD_RESULT result = m_fmodChannel->getPaused(&isPaused);
        if (result != FMOD_OK)
        {
            LogErrorFMOD(result);
            return;
        }

        if (isPaused)
            return;

        result = m_fmodChannel->setPaused(true);
        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }

    void AudioSource::Stop()
    {
        if (!IsPlaying())
            return;

        FMOD_RESULT result = m_fmodChannel->stop();
        m_fmodChannel = nullptr;

        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }

    void AudioSource::OnUpdate(float deltaTime)
    {
        ComponentUpdatable::OnUpdate(deltaTime);

        if (!IsPlaying())
            return;

        m_fmodChannel->setMute(m_mute);
        m_fmodChannel->setVolume(m_volume);
        m_fmodChannel->setPriority(m_priority);
        m_fmodChannel->setPitch(m_pitch);
        m_fmodChannel->setPan(m_pan);
        m_fmodSound->set3DMinMaxDistance(m_minDistance, m_maxDistance);

        if (m_audioMode == EAudioMode::AUDIO_3D)
        {
            const Maths::Vec3& pos = GetParent()->GetPos();

            FMOD_VECTOR f_mod_pos = { pos.x, pos.y, pos.z };
            FMOD_VECTOR f_mod_vel = { 0.f, 0.f, 0.f };

            FMOD_RESULT result = m_fmodChannel->set3DAttributes(&f_mod_pos, &f_mod_vel);
            if (result != FMOD_OK)
            {
                m_fmodChannel = nullptr;
                LogErrorFMOD(result);
            }
        }
    }

    bool AudioSource::IsPlaying() const
    {
        if (!m_fmodChannel)
            return false;

        bool isPlaying;
        FMOD_RESULT result = m_fmodChannel->isPlaying(&isPlaying);
        if (result != FMOD_OK)
        {
            LogErrorFMOD(result);
            return false;
        }

        return isPlaying;
    }

    void AudioSource::LogErrorFMOD(int result) const
    {
        BAKERS_LOG_ERROR((std::string("Audio: ") + FMOD_ErrorString(static_cast<FMOD_RESULT>(result))).c_str());
    }

    void AudioSource::CreateSound(const std::string& file_path)
    {
        FMOD_RESULT result = GetRoot()->GetEngine()->GetAudioSystem()->GetFMODSystem()->createSound(file_path.c_str(), static_cast<FMOD_MODE>(m_audioMode), 0, &m_fmodSound);
        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }
}
