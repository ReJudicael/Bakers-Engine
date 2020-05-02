#pragma once

#include "ComponentBase.h"
#include "CoreMinimal.h"

namespace Core::Audio
{
	BAKERS_API_CLASS AudioSource : public Datastructure::ComponentBase
	{
	private:
		std::string m_audioClip;
		bool m_mute{ false };
		bool m_playOnAwake{ true };
		bool m_loop{ false };
		int m_priority{ 128 };
		float m_volume{ 1.f };
		float m_pitch{ 1.f };
		float m_stereoPan{ 0.f };

	public:
		AudioSource();
		~AudioSource();

	protected:
		virtual void	StartCopy(IComponent*& copyTo) const override;

		virtual void	OnCopy(IComponent* copyTo) const override;

		virtual void	OnReset() override;

		virtual void	OnDestroy() override;

		REGISTER_CLASS(Datastructure::ComponentBase);
	};
}
