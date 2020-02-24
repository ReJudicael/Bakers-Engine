#include "InputSystem.hpp"
#include "Window.h"

namespace Core::SystemManagement
{
	InputSystem::InputSystem(Window* window) :
		m_window{ window }
	{
		m_keyPressedListenerID = m_window->OnPressKey += std::bind(&InputSystem::SetKeyDown, this, std::placeholders::_1);
		m_keyReleasedListenerID = m_window->OnReleaseKey += std::bind(&InputSystem::SetKeyUp, this, std::placeholders::_1);
		m_mouseButtonPressedListenerID = m_window->OnPressMouseButton += std::bind(&InputSystem::SetMouseButtonDown, this, std::placeholders::_1);
		m_mouseButtonReleasedListenerID = m_window->OnReleaseMouseButton += std::bind(&InputSystem::SetMouseButtonUp, this, std::placeholders::_1);
		m_scrollListenerID = m_window->OnScrollYAxis += std::bind(&InputSystem::SetScrollYAxis, this, std::placeholders::_1);
	}

	InputSystem::~InputSystem()
	{
		m_window->OnPressKey -= m_keyPressedListenerID;
		m_window->OnReleaseKey -= m_keyReleasedListenerID;
		m_window->OnPressMouseButton -= m_mouseButtonPressedListenerID;
		m_window->OnReleaseMouseButton -= m_mouseButtonReleasedListenerID;
		m_window->OnScrollYAxis -= m_scrollListenerID;
	}

	Core::Maths::Vec2 SystemManagement::InputSystem::GetMousePos() const noexcept
	{
		double pos[2];
		glfwGetCursorPos(m_window->GetGLFWwindow(), &pos[0], &pos[1]);
		return Core::Maths::Vec2(pos[0], pos[1]);
	}
}
