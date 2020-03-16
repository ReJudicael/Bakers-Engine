#pragma once

#include "IComponent.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class Object;

	/**
	 * Default implementation of a base component. Every component should inherit it.
	 * Does absolutly nothing
	 */
	BAKERS_API_CLASS ComponentBase : public virtual IComponent
	{
	public:
		ComponentBase(const ComponentBase& c) = default;
		ComponentBase() = default;
		~ComponentBase() = default;
	};
}
