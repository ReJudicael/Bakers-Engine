#include "InputSystem.hpp"
#include "Window.h"

namespace Core::SystemManagement
{
	InputSystem::InputSystem(Window* window) :
		m_window{ window }
	{
		m_keyPressedListenerID = m_window->OnPressKey += BIND_EVENT(InputSystem::SetKeyDown);
		m_keyReleasedListenerID = m_window->OnReleaseKey += BIND_EVENT(InputSystem::SetKeyUp);
		m_mouseButtonPressedListenerID = m_window->OnPressMouseButton += BIND_EVENT(InputSystem::SetMouseButtonDown);
		m_mouseButtonReleasedListenerID = m_window->OnReleaseMouseButton += BIND_EVENT(InputSystem::SetMouseButtonUp);
		m_scrollListenerID = m_window->OnScrollYAxis += BIND_EVENT(InputSystem::SetScrollYAxis);
	}

	InputSystem::~InputSystem()
	{
		m_window->OnPressKey -= m_keyPressedListenerID;
		m_window->OnReleaseKey -= m_keyReleasedListenerID;
		m_window->OnPressMouseButton -= m_mouseButtonPressedListenerID;
		m_window->OnReleaseMouseButton -= m_mouseButtonReleasedListenerID;
		m_window->OnScrollYAxis -= m_scrollListenerID;
	}
}
