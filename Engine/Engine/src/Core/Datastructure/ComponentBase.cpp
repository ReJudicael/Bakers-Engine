#include "ComponentBase.h"

RTTR_PLUGIN_REGISTRATION
{
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
		copyTo = new ComponentBase();
		OnCopy(copyTo);
	}

	ComponentBase* ComponentBase::GetCopy() const
	{
		IComponent* result;
		StartCopy(result);
		return dynamic_cast<ComponentBase*>(result);
	}

	void	ComponentBase::OnReset()
	{
		IComponent::OnReset();
	}
}