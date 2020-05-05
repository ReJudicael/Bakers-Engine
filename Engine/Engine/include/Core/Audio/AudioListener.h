#pragma once

#include "ComponentUpdatable.h"
#include "CoreMinimal.h"

namespace Core::Audio
{
	BAKERS_API_CLASS AudioListener : public Datastructure::ComponentUpdatable
	{
	private:
		/**
		 * Wheter the component is active or not
		 */
		bool m_isActive{ true };

	public:
		/**
		 * Default constructor
		 */
		AudioListener();

	protected:
		virtual void	StartCopy(IComponent*& copyTo) const override;
		virtual bool	OnStart() override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	OnReset() override;
		virtual void	OnDestroy() override;
		virtual void	OnUpdate(float deltaTime) override;

		REGISTER_CLASS(ComponentUpdatable);
	};
}
