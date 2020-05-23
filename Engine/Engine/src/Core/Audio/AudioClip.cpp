#include <fmod.hpp>
#include <fmod_errors.h>

#include "AudioClip.h"
#include "AudioSystem.h"
#include "RootObject.hpp"
#include "EngineCore.h"

namespace Core::Audio
{
	AudioClip::AudioClip(const std::string& soundPath, FMOD::System* fmodSystem) :
		m_soundPath{ soundPath },
		m_fmodSystem{ fmodSystem }
	{
		CreateSound();
	}

	AudioClip::~AudioClip()
	{
		Release();
	}

	void AudioClip::CreateSound()
	{
		FMOD_RESULT result = m_fmodSystem->createSound(m_soundPath.c_str(), 0, 0, &m_fmodSound);
		CHECK_ERR_FMOD(result);
	}

	void AudioClip::Play(FMOD::Channel** fmodChannel)
	{
		if (!m_fmodSound)
			return;

		FMOD_RESULT result = m_fmodSystem->playSound(m_fmodSound, nullptr, false, fmodChannel);
		CHECK_ERR_FMOD(result);
	}

	void AudioClip::Release()
	{
		if (!m_fmodSound)
			return;

		FMOD_RESULT result = m_fmodSound->release();
		CHECK_ERR_FMOD(result);
	}

	FMOD::Sound* AudioClip::GetFmodSound()
	{
		return m_fmodSound;
	}
};
