#include "IComponent.hpp"
#include "InputSystem.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
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
		ZoneScoped
#ifdef TRACY_ENABLE
		{
			std::string s;
			{
				ZoneScopedN("Getting rttr name for tracy zones")
					ZoneText("This zone might leak data from rttr to generate the required data. It is intended, and a purposeful leak", 105)
				s = std::string("Init of a ") + get_type().get_name().to_string() + std::string(" component");
			}
			ZoneText(s.c_str(), s.size())
		}
#endif
		if (!m_isInit)
			OnInit();
		if (!m_isStarted)
			m_root->AddStart(this);
		m_isInit = true;
	}

	void	IComponent::Start()
	{
		ZoneScoped
#ifdef TRACY_ENABLE
		{ 
			std::string s;
			{
			ZoneScopedN("Getting rttr name for tracy zones")
				ZoneText("This zone might leak data from rttr to generate the required data. It is intended, and a purposeful leak", 105)
				s = std::string("Start of a ") + get_type().get_name().to_string() + std::string(" component");
			}
			ZoneText(s.c_str(), s.size())
		}
#endif
		if (m_isStarted || !m_isActive || IsDestroyed() || !m_parent->IsActive())
			return;
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