#pragma once

#include "ComponentBase.h"
#include "IUpdatable.hpp"

namespace Core::Datastructure
{
	class ComponentUpdatable : public ComponentBase, public virtual IUpdatable
	{
	};
}
