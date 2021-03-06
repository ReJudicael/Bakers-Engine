#include "IUpdatable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Datastructure;
	registration::class_<IUpdatable>("IUpdatable")
		.method("Update", &IUpdatable::Update)
		.property("IsUpdating", &IUpdatable::m_isUpdating, detail::protected_access());
}

namespace Core::Datastructure
{
	bool IUpdatable::OnStart()
	{
		ZoneScoped
		if (GetRoot() == nullptr)
			return false;
		GetRoot()->AddUpdatable(this);
		return IComponent::OnStart();
	}

	IUpdatable::~IUpdatable()
	{
		if (GetRoot() == nullptr)
			return;
		GetRoot()->RemoveUpdatable(this);
	}

	void	IUpdatable::Update(float deltaTime)
	{
		ZoneScoped
			ZoneText("Update of a component", 22)
		if (m_isUpdating && m_isActive && !IsDestroyed() && m_parent->IsActive())
			OnUpdate(deltaTime);
	}

	void	IUpdatable::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
		IComponent::OnCopy(copyTo);
	}

	void	IUpdatable::OnDestroy()
	{
		GetRoot()->RemoveUpdatable(this);
	}

	void	IUpdatable::OnReset()
	{
		IComponent::OnReset();
		m_isUpdating = true;
		GetRoot()->RemoveUpdatable(this);
	}
}