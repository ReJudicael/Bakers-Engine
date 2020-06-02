#include "ComponentUpdatable.h"

namespace Core::Datastructure
{
	RTTR_PLUGIN_REGISTRATION
	{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
		registration::class_<Core::Datastructure::ComponentUpdatable>("Component Updatable")
		.constructor();
	}

	void	ComponentUpdatable::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
		ComponentBase::OnCopy(copyTo);
		IUpdatable::OnCopy(copyTo);
	}

	void	ComponentUpdatable::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
		copyTo = new ComponentUpdatable();
		OnCopy(copyTo);
	}

	void	ComponentUpdatable::OnReset()
	{
		IUpdatable::OnReset();
	}
}
