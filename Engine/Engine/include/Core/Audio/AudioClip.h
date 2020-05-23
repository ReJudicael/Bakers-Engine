#pragma once

#include <string>

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace Core::Audio
{
	/**
	 * An AudioClip stores the audio file
	 */
	class AudioClip
	{
	private:
		/**
		 * Sound path
		 */
		std::string m_soundPath = "";

		/**
		 * The primary central class of FMOD
		 */
		FMOD::System* m_fmodSystem{ nullptr };

		/**
		 * Represents the actual data source of audio, usually an audio file or stream
		 */
		FMOD::Sound* m_fmodSound{ nullptr };

	public:
		/**
		 * Value constructor
		 * @param soundPath: Sound path
		 * @param fmodSystem: FMOD System
		 */
		AudioClip(const std::string& soundPath, FMOD::System* fmodSystem);

		/**
		 * Default constructor
		 */
		~AudioClip();

	public:
		/**
		 * Create Sound
		 */
		void CreateSound();

		/**
		 * Play Sound
		 * @param fmodChannel: FMOD Channel
		 */
		void Play(FMOD::Channel** fmodChannel);

		/**
		 * Release
		 */
		void Release();

	public:
		/**
		 * Get FMOD Sound
		 */
		FMOD::Sound* GetFmodSound();
	};
}
