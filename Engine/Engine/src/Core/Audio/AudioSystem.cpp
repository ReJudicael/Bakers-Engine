#include "AudioSystem.h"
#include "Transform.hpp"

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
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return false;
		}

		int numDrivers{ 0 };
		result = m_fmodSystem->getNumDrivers(&numDrivers);
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return false;
		}

		result = m_fmodSystem->init(m_maxChannels, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return false;
		}

		result = m_fmodSystem->set3DSettings(1.f, 1.f, 1.f);
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return false;
		}

		return true;
	}

	void AudioSystem::Destroy()
	{
		FMOD_RESULT result = m_fmodSystem->close();
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return;
		}

		result = m_fmodSystem->release();
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
		}
	}

	void AudioSystem::Tick()
	{
		FMOD_RESULT result = m_fmodSystem->update();
		if (result != FMOD_OK)
		{
			LogErrorFMOD(result);
			return;
		}

		if (m_listenerTransform)
		{
			Core::Maths::Vec3 pos = m_listenerTransform->GetLocalPos();

			// TODO: (pos-lastpos) / time_taken_since_last_frame_in_seconds.
			Core::Maths::Vec3 vel = { 0.f, 0.f, 0.f };

			Core::Maths::Vec3 forward = m_listenerTransform->GetForward();
			Core::Maths::Vec3 up = m_listenerTransform->GetUp();

			// Set 3D attributes
			result = m_fmodSystem->set3DListenerAttributes(
				0,
				reinterpret_cast<FMOD_VECTOR*>(&pos),
				reinterpret_cast<FMOD_VECTOR*>(&vel),
				reinterpret_cast<FMOD_VECTOR*>(&forward),
				reinterpret_cast<FMOD_VECTOR*>(&up)
			);

			if (result != FMOD_OK)
			{
				LogErrorFMOD(result);
				return;
			}
		}
	}

	void AudioSystem::SetListenerTransform(Core::Datastructure::Transform* listenerTransform)
	{
		m_listenerTransform = listenerTransform;
	}

	FMOD::System* AudioSystem::GetFMODSystem() const
	{
		return m_fmodSystem;
	}

	void AudioSystem::LogErrorFMOD(int error) const
	{
		BAKERS_LOG_ERROR(FMOD_ErrorString(static_cast<FMOD_RESULT>(error)));
	}
}
