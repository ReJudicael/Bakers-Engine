#pragma once

#include "CoreMinimal.h"

#define CHECK_ERR_FMOD(err) Core::Audio::AudioSystem::CheckErrorFMOD(err)

namespace FMOD
{
	class System;
}

namespace Core::Datastructure
{
	class Object;
}

namespace Core
{
	/**
	 * Contain all sound-related classes
	 */
	namespace Audio
	{
		/**
		 * System handling sounds
		 */
		BAKERS_API_CLASS AudioSystem final
		{
		private:
			/**
			 * FMOD system
			 */
			FMOD::System* m_fmodSystem{ nullptr };

			/**
			 * Max channels
			 */
			int m_maxChannels{ 32 };

			/**
			 * Distance factor
			 */
			float m_distanceFactor{ 1.f };

			/**
			 * Listener object
			 */
			Core::Datastructure::Object* m_listener{ nullptr };

		public:
			/**
			 * Constructor
			 */
			AudioSystem();

			/**
			 * Destructor
			 */
			~AudioSystem();

		private:
			/**
			 * Init audio system
			 * @return Whether the initialisation has succeeded or not
			 */
			bool Init();

			/**
			 * Destroy audio system
			 */
			void Destroy();

		public:
			/**
			 * Update audio system
			 */
			void Tick();

			/**
			 * Get FMOD system
			 * @return FMOD system
			 */
			FMOD::System* GetFMODSystem() const;

			/**
			 * Check Error FMOD
			 * @param result: Error
			 * @return True if there is an any error, false otherwise
			 */
			static bool CheckErrorFMOD(int result);
		};
	}
}