#pragma once

#include "EventSystem.hpp"
#include "Input.hpp"

namespace Core::SystemManagement
{
	class InputSystem final
	{
	private:
		/* Stockage mouse buttons and keys */
		std::unordered_map<EKey, EStateInput> m_registeredKeys;
		std::unordered_map<EMouseButton, EStateInput> m_registeredMouseButtons;

		/* Events */
		ID m_keyPressedListenerID;
		ID m_keyReleasedListenerID;
		ID m_mouseButtonPressedListenerID;
		ID m_mouseButtonReleasedListenerID;

	public:

	};
}