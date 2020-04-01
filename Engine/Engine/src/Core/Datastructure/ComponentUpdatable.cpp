#include "ComponentUpdatable.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Datastructure::ComponentUpdatable>("ComponentUpdatable")
		.constructor();
}

namespace Core::Datastructure
{
	void	ComponentUpdatable::OnCopy(void* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		ComponentUpdatable* copy{ (ComponentUpdatable*)copyTo };
	}

	void	ComponentUpdatable::StartCopy(void*& copyTo) const
	{
		copyTo = new ComponentUpdatable();
		OnCopy(copyTo);
	}
}
