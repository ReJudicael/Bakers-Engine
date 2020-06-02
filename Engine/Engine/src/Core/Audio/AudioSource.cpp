#include <fmod.hpp>
#include <fmod_errors.h>

#include "AudioSource.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "AudioSystem.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Datastructure;

	registration::class_<Core::Audio::AudioSource>("Audio Source")
		.enumeration<Core::Audio::EAudioMode>("Audio Mode")
		(
			value("2D", Core::Audio::EAudioMode::AUDIO_2D),
			value("3D", Core::Audio::EAudioMode::AUDIO_3D)
		)

		.constructor()
		.property("Audio Clip",     &Core::Audio::AudioSource::m_audioClipStr)
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
		ZoneScoped
		copyTo = new AudioSource();
		OnCopy(copyTo);
	}

	void AudioSource::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
		ComponentUpdatable::OnCopy(copyTo);
		AudioSource* copy{ dynamic_cast<AudioSource*>(copyTo) };

		copy->m_audioClipStr	= m_audioClipStr;
		copy->m_audioMode	= m_audioMode;
		copy->m_mute		= m_mute;
		copy->m_playOnAwake	= m_playOnAwake;
		copy->m_loop		= m_loop;
		copy->m_priority	= m_priority;
		copy->m_volume		= m_volume;
		copy->m_pitch		= m_pitch;
		copy->m_pan			= m_pan;
		copy->m_minDistance	= m_minDistance;
		copy->m_maxDistance	= m_maxDistance;
	}

	void AudioSource::OnInit()
	{
		ComponentUpdatable::OnInit();
		m_fmodSystem = GetRoot()->GetEngine()->GetAudioSystem()->GetFMODSystem();
	}

	void AudioSource::OnReset()
	{
		ComponentUpdatable::OnReset();

		m_audioClipStr	= "";
		m_audioMode		= Core::Audio::EAudioMode::AUDIO_2D;
		m_mute			= false;
		m_playOnAwake	= true;
		m_loop			= false;
		m_priority		= 128;
		m_volume		= 1.f;
		m_pitch			= 1.f;
		m_pan			= 0.f;
		m_minDistance	= 1.f;
		m_maxDistance	= 1000.f;
	}

	void AudioSource::OnDestroy()
	{
		ComponentUpdatable::OnDestroy();
		Stop();
	}

	bool AudioSource::OnStart()
	{
		ZoneScoped
		if (m_audioClipStr.empty())
			return false;

		m_audioClip = GetRoot()->GetEngine()->GetResourcesManager()->CreateAudioClip(m_audioClipStr, m_fmodSystem);

		if (m_playOnAwake)
			Play();

		return ComponentUpdatable::OnStart();
	}

	void AudioSource::OnUpdate(float deltaTime)
	{
		ComponentUpdatable::OnUpdate(deltaTime);
		Update();
	}

	void AudioSource::Play()
	{
		if (IsPlaying())
			return;

		m_audioClip->Play(&m_fmodChannel);
		m_fmodChannel->setMute(true);
	}

	void AudioSource::Pause()
	{
		if (!IsPlaying())
			return;

		bool isPaused;
		FMOD_RESULT result = m_fmodChannel->getPaused(&isPaused);
		if (CHECK_ERR_FMOD(result))
			return;

		if (isPaused)
			return;

		result = m_fmodChannel->setPaused(true);
		CHECK_ERR_FMOD(result);
	}

	void AudioSource::Stop()
	{
		m_fmodChannel->stop();
		m_fmodChannel = nullptr;
	}

	void AudioSource::Update()
	{
		if (!IsPlaying())
			return;

		m_fmodChannel->setMode(static_cast<FMOD_MODE>(m_audioMode) | (m_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
		m_fmodChannel->setMute(m_mute);
		m_fmodChannel->setVolume(m_volume);
		m_fmodChannel->setPriority(m_priority);
		m_fmodChannel->setPitch(m_pitch);
		m_fmodChannel->setPan(m_pan);
		m_fmodChannel->set3DMinMaxDistance(m_minDistance, m_maxDistance);

		if (m_audioMode == EAudioMode::AUDIO_3D)
		{
			const Maths::Vec3& pos = GetParent()->GetGlobalPos();

			FMOD_VECTOR f_mod_pos = { pos.x, pos.y, pos.z };
			FMOD_VECTOR f_mod_vel = { 0.f, 0.f, 0.f };

			FMOD_RESULT result = m_fmodChannel->set3DAttributes(&f_mod_pos, &f_mod_vel);
			if (result != FMOD_OK)
				Stop();
		}
	}

	bool AudioSource::IsPlaying()
	{
		if (!m_fmodChannel)
			return false;

		bool isPlaying;
		m_fmodChannel->isPlaying(&isPlaying);

		if (!isPlaying)
		{
			Stop();
			return false;
		}

		return true;
	}
}
