#pragma once

#include "ComponentUpdatable.h"
#include "CoreMinimal.h"

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace Core::Audio
{
	/**
	 * Audio Mode: 2D or 3D
	 */
	enum class EAudioMode
	{
		AUDIO_2D = 8,
		AUDIO_3D = 16
	};

	/**
	 * Component that allows the object to emit sound
	 */
	BAKERS_API_CLASS AudioSource : public Datastructure::ComponentUpdatable
	{
	private:
		/**
		 * The primary central class of FMOD
		 */
		FMOD::System*	m_fmodSystem{ nullptr };

		/**
		 * Represents the actual data source of audio, usually an audio file or stream
		 */
		FMOD::Sound*	m_fmodSound{ nullptr };

		/**
		 * Represents a sound-card channel and a interface to sound playback.
		 */
		FMOD::Channel*	m_fmodChannel{ nullptr };

		/**
		 * AudioClip path
		 */
		std::string		m_audioClip;

		/**
		 * Audio Mode: 2D or 3D
		 */
		EAudioMode		m_audioMode{ EAudioMode::AUDIO_2D };

	private:
		/**
		 * Mute the sound channel
		 */
		bool	m_mute{ false };

		/**
		 * If set to true, the audio source will automatically start playing on awake
		 */
		bool	m_playOnAwake{ true };

		/**
		 * Whether the audio clip replays after it finishes or not
		 */
		bool	m_loop{ false };

		/**
		 * The priority for the channel after it has been played
		 * from 0 (most important) to 256 (least important)
		 */
		int		m_priority{ 128 };

		/**
		 * The volume level linearly
		 */
		float	m_volume{ 1.f };

		/**
		 * The pitch value
		 */
		float	m_pitch{ 1.f };

		/**
		 * The pan value
		 */
		float	m_pan{ 0.f };

		/**
		 * The minimum audible distance
		 */
		float	m_minDistance{ 1.f };

		/**
		 * The maximum audible distance
		 */
		float	m_maxDistance{ 10000.f };

	public:
		/**
		 * Default constructor
		 */
		AudioSource();

	protected:
		virtual void StartCopy(IComponent*& copyTo) const override;
		virtual void OnCopy(IComponent* copyTo) const override;
		virtual void OnInit() override;
		virtual void OnReset() override;
		virtual void OnDestroy() override;
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;

	private:
		/**
		 * Play sound
		 */
		void Play();

		/**
		 * Pause sound
		 */
		void Pause();

		/**
		 * Stop sound
		 */
		void Stop();

		/**
		 * Update attributes 
		 */
		void Update();

		/**
		 * Release sound
		 */
		void Destroy();

		/**
		 * Wheter the sound is playing or not
		 * @return Wheter the sound is playing or not
		 */
		bool IsPlaying() const;

		/**
		 * Create a sound
		 * @param soundPath: Path of the sound
		 */
		void CreateSound(const std::string& soundPath);

		REGISTER_CLASS(ComponentUpdatable);
	};
}
