#include "IUpdatable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<IUpdatable>("IUpdatable")
		.method("Update", &IUpdatable::Update)
		.property("IsUpdating", &IUpdatable::m_isUpdating, detail::protected_access());
}

namespace Core::Datastructure
{
	void IUpdatable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddUpdatable(this);
	}

	IUpdatable::~IUpdatable()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->RemoveUpdatable(this);
	}

	void	IUpdatable::Update(float deltaTime)
	{
		ZoneScoped
			ZoneText("Update of a component", 22)
		if (m_isUpdating && m_isActive && !IsDestroyed())
			OnUpdate(deltaTime);
	}

	void	IUpdatable::OnCopy(void* copyTo) const
	{
		IComponent::OnCopy(copyTo);
	}

	void	IUpdatable::OnDestroy()
	{
		GetScene()->RemoveUpdatable(this);
	}
}