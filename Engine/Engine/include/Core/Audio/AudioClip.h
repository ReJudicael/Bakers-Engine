#pragma once

#include <fmod.hpp>

namespace Core::Datastructure
{
	class Transform;
}

namespace Core::Audio
{
	class AudioClip
	{
	private:
		Core::Datastructure::Transform* m_transform;
		FMOD::System*	m_fmodSystem;
		FMOD::Channel*	m_fmodChannel;
		FMOD::Sound*	m_fmodSound;
		int				m_loop;
		float			m_minDistance;
		float			m_maxDistance;

		bool m_mute{ false };
		bool m_playOnAwake{ true };
		int m_priority{ 128 };
		float m_volume{ 1.f };
		float m_pitch{ 1.f };
		float m_stereoPan{ 0.f };

	public:
		void Play();
		void Pause();
		void Stop();

		void SetTransform(Core::Datastructure::Transform* transform);
		void SetMute(bool mute);
		void SetLoop(bool loop);
		void SetPriority(int priority);
		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetStereoPan(float pan);

		
	};
}
