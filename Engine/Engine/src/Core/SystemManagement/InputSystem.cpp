#include "InputSystem.hpp"
#include "EngineCore.h"

namespace Core::SystemManagement
{
	RTTR_PLUGIN_REGISTRATION
	{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
		Core::Datastructure::lua.new_usertype<InputSystem>("InputSystem");
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsKeyPressed", &InputSystem::IsKeyPressed);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsKeyDown", &InputSystem::IsKeyDown);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsKeyUp", &InputSystem::IsKeyUp);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsKeyUnused", &InputSystem::IsKeyUnused);
		
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsMouseButtonPressed", &InputSystem::IsMouseButtonPressed);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsMouseButtonDown", &InputSystem::IsMouseButtonDown);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsMouseButtonUp", &InputSystem::IsMouseButtonUp);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsMouseButtonUnused", &InputSystem::IsMouseButtonUnused);
		Core::Datastructure::RegisterClassMethod<InputSystem>("InputSystem", "IsMouseButtonDoubleClicked", &InputSystem::IsMouseDoubleClicked);
	}


	InputSystem::InputSystem(Core::Datastructure::EngineCore* core) :
		m_core{ core }
	{
		m_keyPressedListenerID = m_core->OnPressKey += BIND_EVENT(InputSystem::SetKeyDown);
		m_keyReleasedListenerID = m_core->OnReleaseKey += BIND_EVENT(InputSystem::SetKeyUp);
		m_mouseButtonPressedListenerID = m_core->OnPressMouseButton += BIND_EVENT(InputSystem::SetMouseButtonDown);
		m_mouseButtonReleasedListenerID = m_core->OnReleaseMouseButton += BIND_EVENT(InputSystem::SetMouseButtonUp);
		m_scrollListenerID = m_core->OnScrollYAxis += BIND_EVENT(InputSystem::SetScrollYAxis);
	}

	InputSystem::~InputSystem()
	{
		m_core->OnPressKey -= m_keyPressedListenerID;
		m_core->OnReleaseKey -= m_keyReleasedListenerID;
		m_core->OnPressMouseButton -= m_mouseButtonPressedListenerID;
		m_core->OnReleaseMouseButton -= m_mouseButtonReleasedListenerID;
		m_core->OnScrollYAxis -= m_scrollListenerID;
	}

	Core::Maths::Vec2 InputSystem::GetMousePos() const noexcept
	{
		if (!m_isActive)
			return { 0, 0 };
		return m_core->GetMousePos();
	}

	Core::Maths::Vec2 InputSystem::GetMousePosInWindow() const noexcept
	{
		if (!m_isActive)
			return { 0, 0 };
		return m_core->GetMousePosInWindow();
	}

	void InputSystem::SetMousePos(Core::Maths::Vec2 pos) const noexcept
	{
		if (!m_isActive)
			return;
		glfwSetCursorPos(m_core->GetWindow(), pos.x, pos.y);
	}

	void InputSystem::RegisterInputEnums(sol::state& lua)
	{
		lua["MouseButton"] = lua.create_table_with(
			"B1", EMouseButton::B1,
			"B2", EMouseButton::B2,
			"B3", EMouseButton::B3,
			"LEFT", EMouseButton::LEFT,
			"RIGHT", EMouseButton::RIGHT,
			"MIDDLE", EMouseButton::MIDDLE,
			"LAST", EMouseButton::LAST);

		lua["Key"] = lua.create_table_with(
			"A", EKey::A,
			"B", EKey::B,
			"C", EKey::C,
			"D", EKey::D,
			"E", EKey::E,
			"F", EKey::F,
			"G", EKey::G,
			"H", EKey::H,
			"I", EKey::I,
			"J", EKey::J,
			"K", EKey::K,
			"L", EKey::L,
			"M", EKey::M,
			"N", EKey::N,
			"O", EKey::O,
			"P", EKey::P,
			"Q", EKey::Q,
			"R", EKey::R,
			"S", EKey::S,
			"T", EKey::T,
			"U", EKey::U,
			"V", EKey::V,
			"W", EKey::W,
			"X", EKey::X,
			"Y", EKey::Y,
			"Z", EKey::Z
		);
	}
}
