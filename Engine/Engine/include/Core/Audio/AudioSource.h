#pragma once

#include "ComponentUpdatable.h"
#include "CoreMinimal.h"

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

namespace Core::Audio
{
	BAKERS_API_CLASS AudioSource : public Datastructure::ComponentUpdatable
	{
	private:
		std::string m_audioClip;

		FMOD::System* m_fmodSystem{ nullptr };
		FMOD::Sound* m_fmodSound{ nullptr };
		FMOD::Channel* m_fmodChannel{ nullptr };

		bool m_mute{ false };
		bool m_playOnAwake{ true };
		bool m_loop{ false };
		int m_priority{ 128 };
		float m_volume{ 1.f };
		float m_pitch{ 1.f };
		float m_stereoPan{ 0.f };
		float m_minDistance{ 1.f };
		float m_maxDistance{ 100.f };

	public:
		AudioSource();
		~AudioSource();

	protected:
		virtual void	StartCopy(IComponent*& copyTo) const override;

		virtual void	OnCopy(IComponent* copyTo) const override;

		virtual void	OnReset() override;

		virtual void	OnDestroy() override;

		virtual void OnStart() override;

		/**
		 * Call Update function in Lua script
		 * @param deltaTime: Time elapsed between two frames, sent to Lua Update
		 */
		virtual void OnUpdate(float deltaTime) override;

		void Play();
		void Pause();
		void Stop();
		bool IsPlaying() const;
		void LogErrorFMOD(int result) const;
		void CreateSound(const std::string& file_path);

		REGISTER_CLASS(Datastructure::ComponentUpdatable);
	};
}
