#pragma once

#include "ComponentBase.h"
#include "IUpdatable.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	/**
	 * Default updatable component implementation. Does absolutly nothing
	 */
	BAKERS_API_CLASS ComponentUpdatable : public ComponentBase, public virtual IUpdatable
	{
	protected:
		virtual void	OnCopy(void* copyTo) const override;
		virtual void	StartCopy(void*& copyTo) const override;

	public:
		virtual void OnUpdate(float deltaTime) override {};

		REGISTER_CLASS(ComponentBase, IUpdatable)
	};
}
