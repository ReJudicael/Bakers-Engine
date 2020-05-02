#pragma once

#include "ComponentBase.h"
#include "CoreMinimal.h"

namespace Core::Audio
{
	BAKERS_API_CLASS AudioListener : public Datastructure::ComponentBase
	{
	private:
		bool m_isActive{ true };

	public:
		AudioListener();
		~AudioListener();

	protected:
		virtual void	StartCopy(IComponent*& copyTo) const override;

		virtual void	OnCopy(IComponent* copyTo) const override;

		virtual void	OnReset() override;

		virtual void	OnDestroy() override;

		REGISTER_CLASS(Datastructure::ComponentBase);
	};
}
