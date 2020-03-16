#include "IUpdatable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<IUpdatable>("IUpdatable")
		.method("Update", &IUpdatable::Update)
		.property("IsUpdating", &IUpdatable::m_isUpdating, detail::protected_access());
}

namespace Core::Datastructure
{
	void Datastructure::IUpdatable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddUpdatable(this);
	}

	void	IUpdatable::Update(float deltaTime)
	{
		ZoneScoped
			ZoneText("Update of a component", 22)
		if (m_isUpdating && m_isActive)
			OnUpdate(deltaTime);
	}
}