#include "AudioSystem.h"
#include "Transform.hpp"

#include <fmod_errors.h>

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
		m_fmodResult = FMOD::System_Create(&m_fmodSystem);
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
			return false;
		}

		int numDrivers{ 0 };
		m_fmodResult = m_fmodSystem->getNumDrivers(&numDrivers);
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
			return false;
		}

		m_fmodResult = m_fmodSystem->init(m_maxChannels, FMOD_INIT_NORMAL, nullptr);
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
			return false;
		}

		m_fmodResult = m_fmodSystem->set3DSettings(1.f, m_distanceFactor, 1.f);
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
			return false;
		}

		return true;
	}

	void AudioSystem::Destroy()
	{
		m_fmodResult = m_fmodSystem->close();
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
			return;
		}

		m_fmodResult = m_fmodSystem->release();
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
		}
	}

	void AudioSystem::Tick()
	{
		m_fmodResult = m_fmodSystem->update();
		if (m_fmodResult != FMOD_OK)
		{
			LogErrorFMOD(m_fmodResult);
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
			m_fmodResult = m_fmodSystem->set3DListenerAttributes(
				0,
				reinterpret_cast<FMOD_VECTOR*>(&pos),
				reinterpret_cast<FMOD_VECTOR*>(&vel),
				reinterpret_cast<FMOD_VECTOR*>(&forward),
				reinterpret_cast<FMOD_VECTOR*>(&up)
			);

			if (m_fmodResult != FMOD_OK)
			{
				LogErrorFMOD(m_fmodResult);
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

	void AudioSystem::LogErrorFMOD(FMOD_RESULT error) const
	{
		BAKERS_LOG_ERROR(FMOD_ErrorString(error));
	}
}
