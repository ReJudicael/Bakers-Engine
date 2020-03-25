#include "InputSystem.hpp"
#include "EngineCore.h"

namespace Core::SystemManagement
{
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

	Core::Maths::Vec2 SystemManagement::InputSystem::GetMousePos() const noexcept
	{
		return m_core->GetMousePos();
	}

	void SystemManagement::InputSystem::SetMousePos(Core::Maths::Vec2 pos) const noexcept
	{
		glfwSetCursorPos(m_core->GetWindow(), pos.x, pos.y);
	}

	void SystemManagement::InputSystem::SetMouseAppearance(ECursorAppearance type) const noexcept
	{
		switch (type)
		{
		case ECursorAppearance::DEFAULT:
			glfwSetInputMode(m_core->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
		case ECursorAppearance::INVISIBLE:
			glfwSetInputMode(m_core->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
		}
	}
}
