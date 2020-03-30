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
	void	ComponentBase::StartCopy(void*& copyTo) const
	{
		copyTo = new ComponentBase();
		OnCopy(copyTo);
	}

	ComponentBase* ComponentBase::GetCopy() const
	{
		void* result;
		StartCopy(result);
		return (ComponentBase*)result;
	}
}