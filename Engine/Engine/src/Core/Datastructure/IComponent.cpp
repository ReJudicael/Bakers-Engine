#include "IComponent.hpp"
#include "InputSystem.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	using namespace Core::Datastructure;
	registration::class_<IComponent>("IComponent")
		.property_readonly("parent", &IComponent::GetParent, detail::protected_access())
		.property_readonly("Scene", &IComponent::GetRoot, detail::protected_access())
		.property_readonly("InputManager", &IComponent::Input, detail::protected_access())
		.method("Start", &IComponent::Start)
		.method("Destroy", &IComponent::Destroy)
		.property_readonly("IsDestroyed", &IComponent::IsDestroyed, detail::protected_access())
		.property_readonly("IsStarted", &IComponent::IsStarted, detail::protected_access())
		.property("IsActive", &IComponent::m_isActive, detail::protected_access());
}

namespace Core::Datastructure
{
	void	IComponent::Init() noexcept
	{
		if (!m_isInit)
			OnInit();
		if (!m_isStarted)
			m_root->AddStart(this);
		m_isInit = true;
	}

	void	IComponent::Start()
	{
		if (m_isStarted || !m_isActive || IsDestroyed() || !m_parent->IsActive())
			return;
		ZoneScoped
			ZoneText("Start of a component", 21)
		if (OnStart())
			m_isStarted = true;
	}

	void IComponent::Destroy()
	{
		if (m_isDestroyed)
			return;
		ZoneScoped
			ZoneText("Destroy of a component", 23)
		OnDestroy();
		m_root->DestroyComponent(this);
		m_isDestroyed = true;
	}

	SystemManagement::InputSystem* IComponent::Input() noexcept
	{
		return m_root->GetInput();
	}

	void	IComponent::OnReset()
	{
		m_isStarted = false;
		m_root->AddStart(this);
		m_isActive = true;
	}

	void	IComponent::Reset()
	{
		if (m_isDestroyed)
			return; //We do not reset a destroyed component

		ZoneScoped
			ZoneText("Reset of a component", 21)

		OnReset();
	}
}