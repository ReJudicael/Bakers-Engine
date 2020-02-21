#pragma once

#include "IComponent.hpp"

namespace Core::Datastructure
{
	class Object;

	/**
	 * Default implementation of a base component. Every component should inherit it.
	 * Does absolutly nothing
	 */
	class ComponentBase : public virtual IComponent
	{
	public:
		ComponentBase(const ComponentBase& c) = default;
		ComponentBase() = default;
		~ComponentBase() = default;
	};
}
