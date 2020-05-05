#include "AudioSystem.h"
#include "Object.hpp"

#include <fmod_errors.h>
#include <fmod.hpp>

namespace Core::Audio
{
	AudioSystem::AudioSystem()
	{
		Init();
	}

	AudioSystem::~AudioSystem()
	{
		Destroy();
	}

	bool AudioSystem::Init()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
		if (CHECK_ERR_FMOD(result))
			return false;

		int numDrivers{ 0 };
		result = m_fmodSystem->getNumDrivers(&numDrivers);
		if (CHECK_ERR_FMOD(result))
			return false;

		result = m_fmodSystem->init(m_maxChannels, FMOD_INIT_NORMAL, nullptr);
		if (CHECK_ERR_FMOD(result))
			return false;

		result = m_fmodSystem->set3DSettings(1.f, 1.f, 1.f);
		if (CHECK_ERR_FMOD(result))
			return false;

		return true;
	}

	void AudioSystem::Destroy()
	{
		FMOD_RESULT result = m_fmodSystem->close();
		if (CHECK_ERR_FMOD(result))
			return;

		result = m_fmodSystem->release();
		CHECK_ERR_FMOD(result);
	}

	void AudioSystem::Tick()
	{
		FMOD_RESULT result = m_fmodSystem->update();
		if (CHECK_ERR_FMOD(result))
			return;
	}

	FMOD::System* AudioSystem::GetFMODSystem() const
	{
		return m_fmodSystem;
	}

	bool AudioSystem::CheckErrorFMOD(int result)
	{
		if (result != FMOD_OK)
		{
			BAKERS_LOG_ERROR((std::string("Audio: ") + FMOD_ErrorString(static_cast<FMOD_RESULT>(result))).c_str());
			return true;
		}

		return false;
	}
}
