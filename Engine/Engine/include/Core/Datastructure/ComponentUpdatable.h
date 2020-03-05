#pragma once

#include "ComponentBase.h"
#include "IUpdatable.hpp"
#include "Debug.h"
#include "Reflection.h"

namespace Core::Datastructure
{
	/**
	 * Default updatable component implementation. Does absolutly nothing
	 */
	class ComponentUpdatable : public ComponentBase, public virtual IUpdatable
	{
		REGISTER_CLASS(ComponentBase, IUpdatable)
	};
}
