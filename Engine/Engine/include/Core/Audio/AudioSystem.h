#pragma once

#include <fmod.hpp>

namespace Core::Datastructure
{
	class Transform;
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
		class AudioSystem
		{
		private:
			/**
			 * FMOD system
			 */
			FMOD::System* m_fmodSystem{ nullptr };
			/**
			 * State of FMOD
			 */
			FMOD_RESULT m_fmodResult{ FMOD_OK };

			/**
			 * Max channels
			 */
			int m_maxChannels{ 32 };

			/**
			 * Distance factor
			 */
			float m_distanceFactor{ 1.f };

			/**
			 * Listener transform
			 */
			Core::Datastructure::Transform* m_listenerTransform{ nullptr };

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
			 * Set listener transform
			 * @param listenerTransform: Listener transform
			 */
			void SetListenerTransform(Core::Datastructure::Transform* listenerTransform);

		private:
			/**
			 * Display Log error of FMOD
			 * @param error: FMOD error
			 */
			void LogErrorFMOD(FMOD_RESULT error) const;
		};
	}
}