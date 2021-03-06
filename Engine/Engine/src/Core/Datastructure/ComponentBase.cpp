#include "ComponentBase.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Datastructure;
	registration::class_<ComponentBase>("ComponentBase")
		.constructor<>()
		.constructor<const ComponentBase&>()
		.method("GetCopy", &ComponentBase::GetCopy);
}

namespace Core::Datastructure
{
	void	ComponentBase::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
		copyTo = new ComponentBase();
		OnCopy(copyTo);
	}

	ComponentBase* ComponentBase::GetCopy() const
	{
		ZoneScoped
		IComponent* result;
		StartCopy(result);
		return dynamic_cast<ComponentBase*>(result);
	}

	void	ComponentBase::OnReset()
	{
		IComponent::OnReset();
	}
}
