#include "ComponentUpdatable.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Datastructure::ComponentUpdatable>("ComponentUpdatable")
		.constructor();
}

namespace Core::Datastructure
{
	void	ComponentUpdatable::OnCopy(IComponent* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		IUpdatable::OnCopy(copyTo);
	}

	void	ComponentUpdatable::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new ComponentUpdatable();
		OnCopy(copyTo);
	}

	void	ComponentUpdatable::OnReset()
	{
		IUpdatable::OnReset();
	}
}
