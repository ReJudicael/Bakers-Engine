#pragma once

#include "IComponent.hpp"

namespace Core::Datastructure
{
	class Object;

	class ComponentBase : public virtual IComponent
	{
	public:
		ComponentBase(const ComponentBase& c) = default;
		ComponentBase() = default;
		~ComponentBase() = default;
	};
}
