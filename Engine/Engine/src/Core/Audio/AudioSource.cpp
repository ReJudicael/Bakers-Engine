#include <fmod.hpp>
#include <fmod_errors.h>

#include "Object.hpp"
#include "AudioSource.h"


RTTR_PLUGIN_REGISTRATION
{
    using namespace Core::Datastructure;

    registration::class_<Core::Audio::AudioSource>("Audio Source")
        .constructor()
        .property("Audio Clip", &Core::Audio::AudioSource::m_audioClip)
        .property("Mute", &Core::Audio::AudioSource::m_mute)
        .property("Play On Awake", &Core::Audio::AudioSource::m_playOnAwake)
        .property("Loop", &Core::Audio::AudioSource::m_loop)
        .property("Priority", &Core::Audio::AudioSource::m_priority)
        .property("Volume", &Core::Audio::AudioSource::m_volume)
        .property("Pitch", &Core::Audio::AudioSource::m_pitch)
        .property("Stereo Pan", &Core::Audio::AudioSource::m_stereoPan)
        .property("Min Distance", &Core::Audio::AudioSource::m_minDistance)
        .property("Max Distance", &Core::Audio::AudioSource::m_maxDistance);
}

namespace Core::Audio
{
    AudioSource::AudioSource() : ComponentUpdatable()
    {
        FMOD::System_Create(&m_fmodSystem);
        m_fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
    }

    AudioSource::~AudioSource()
    {
        if (!m_fmodSound)
            return;
    
        FMOD_RESULT result = m_fmodSound->release();
        if (result != FMOD_OK)
            LogErrorFMOD(result);
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
        copy->m_isActive = m_isActive;
    }

    void AudioSource::OnReset()
    {
        ComponentUpdatable::OnReset();
    }

    void AudioSource::OnDestroy()
    {
        m_isActive = false;
    }

    void AudioSource::OnStart()
    {
        ComponentUpdatable::OnStart();

        if (m_audioClip.empty())
            return;

        CreateSound(m_audioClip);
        Play();
    }

    void AudioSource::Play()
    {
        if (IsPlaying())
            return;

        FMOD_RESULT result = m_fmodSystem->playSound(m_fmodSound, nullptr, false, &m_fmodChannel);
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

        const auto pos = GetParent()->GetPos();

        FMOD_VECTOR f_mod_pos = { pos.x, pos.y, pos.z };
        FMOD_VECTOR f_mod_vel = { 0, 0, 0 };

        // Set 3D attributes
        FMOD_RESULT result = m_fmodChannel->set3DAttributes(&f_mod_pos, &f_mod_vel);
        if (result != FMOD_OK)
        {
            m_fmodChannel = nullptr;
            LogErrorFMOD(result);
            return;
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
        FMOD_RESULT result = m_fmodSystem->createSound(file_path.c_str(), FMOD_3D | m_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF, nullptr, &m_fmodSound);
        if (result != FMOD_OK)
        {
            LogErrorFMOD(result);
            return;
        }

        result = m_fmodSound->set3DMinMaxDistance(m_minDistance, m_maxDistance);
        if (result != FMOD_OK)
            LogErrorFMOD(result);
    }
}
