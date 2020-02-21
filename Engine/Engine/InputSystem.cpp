#include "InputSystem.hpp"

namespace Core::SystemManagement
{
	InputSystem::InputSystem(Window* window) :
		m_window{ window }
	{ 
		m_keyPressedListenerID = m_window->OnPressKey += std::bind(&SetKeyDown, this, std::placeholders::_1);
		m_keyReleasedListenerID = m_window->OnReleaseKey += std::bind(&SetKeyUp, this, std::placeholders::_1);
		m_mouseButtonPressedListenerID = m_window->OnPressMouseButton += std::bind(&SetMouseButtonDown, this, std::placeholders::_1);
		m_mouseButtonReleasedListenerID = m_window->OnReleaseMouseButton += std::bind(&SetMouseButtonUp, this, std::placeholders::_1);
	}

	InputSystem::~InputSystem()
	{
		m_window->OnPressKey -= m_keyPressedListenerID;
		m_window->OnReleaseKey -= m_keyReleasedListenerID;
		m_window->OnPressMouseButton -= m_mouseButtonPressedListenerID;
		m_window->OnReleaseMouseButton -= m_mouseButtonReleasedListenerID;
	}
}
