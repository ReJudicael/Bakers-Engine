#pragma once

#include "EventSystem.hpp"
#include "Input.hpp"

class Window;

namespace Core::SystemManagement
{
	/**
	 * Handle inputs by storing whether each key is pressed or released
	 */
	class InputSystem final
	{
	private:
		/* Stockage mouse buttons, keys and scroll */
		std::unordered_map<EKey, EStateInput> m_registeredKeys;
		std::unordered_map<EMouseButton, EStateInput> m_registeredMouseButtons;
		EStateScroll m_registeredScrollYAxis{ EStateScroll::UNUSED };

		/* Events */
		ID m_keyPressedListenerID{ 0 };
		ID m_keyReleasedListenerID{ 0 };
		ID m_mouseButtonPressedListenerID{ 0 };
		ID m_mouseButtonReleasedListenerID{ 0 };
		ID m_scrollListenerID{ 0 };

		Window* m_window;

	public:
		/**
		 * Constructor setting events into given window
		 * @param window: Window that will store the key press and release events
		 */
		InputSystem(Window* window);

		/**
		 * Remove each event from the stored window
		 */
		~InputSystem();

		/**
		 * Get the state (pressed or released) of the given key
		 * @param key: Key of which the state will be checked
		 * @return State of the given key
		 */
		EStateInput GetKeyState(EKey key) const noexcept;

		/**
		 * Get the state (pressed or released) of the mouse button
		 * @param button: Button of which the state will be checked
		 * @return State of the given button
		 */
		EStateInput GetMouseButtonState(EMouseButton button) const noexcept;

		/**
		 * Get the state (down or up) of the scroll
		 * @return State of the scroll
		 */
		EStateScroll GetScrollState() const noexcept;

	private:
		/**
		 * Set the value stored for the given key to the value Pressed
		 * @param key: Key that will be indicated as pressed
		 */
		void SetKeyDown(EKey key) noexcept;

		/**
		 * Set the value stored for the given key to the value Released
		 * @param key: Key that will be indicated as released
		 */
		void SetKeyUp(EKey key) noexcept;

		/**
		 * Set the value stored for the given mouse button to the value Pressed
		 * @param button: Mouse button that will be indicated as pressed
		 */
		void SetMouseButtonDown(EMouseButton button) noexcept;

		/**
		 * Set the value stored for the given mouse button to the value Released
		 * @param button: Mouse button that will be indicated as released
		 */
		void SetMouseButtonUp(EMouseButton button) noexcept;

		/**
		 * Set the scroll of y-axis state
		 * @param yoffset: The scroll offset along the y-axis.
		 */
		void SetScrollYAxis(double yoffset) noexcept;

		/**
		 * Clear the recorded keys
		 */
		void ClearRegisteredKeys() noexcept;

		/**
		 * Clear the recorded mouse buttons
		 */
		void ClearRegisteredMouseButtons() noexcept;

		/**
		 * Clear the recorded scroll state
		 */
		void ClearRegisteredScroll() noexcept;

	public:
		/**
		 *  Must be called at the end of each frame
		 *  Clear the recorded inputs
		 */
		void ClearRegisteredInputs() noexcept;

	private:
		/**
		 * Check whether the given key exists in the map or not
		 * @param key: Key to be checked
		 * @return true if the key exists in the map, false otherwise
		 */
		bool IsKeyRegistered(EKey key) const noexcept;

		/**
		 * Check whether the given mouse button exists in the map or not
		 * @param button: Mouse button to be checked
		 * @return true if the key exists in the map, false otherwise
		 */
		bool IsMouseButtonRegistered(EMouseButton button) const noexcept;

	public:
		/**
		 * Indicate whether the given key is pressed or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is pressed during one frame, false otherwise
		 */
		bool IsKeyPressed(EKey key) const noexcept;

		/**
		 * Indicate whether the given key is pressed or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is currently pressed, false otherwise
		 */
		bool IsKeyDown(EKey key) const noexcept;

		/**
		 * Indicate whether the given key is released or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is currently released, false otherwise
		 */
		bool IsKeyUp(EKey key) const noexcept;

		/**
		 * Indicate whether the given mouse button is pressed or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently pressed during one frame, false otherwise
		 */
		bool IsMouseButtonPressed(EMouseButton button) const noexcept;

		/**
		 * Indicate whether the given mouse button is pressed or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently pressed, false otherwise
		 */
		bool IsMouseButtonDown(EMouseButton button) const noexcept;

		/**
		 * Indicate whether the given mouse button is released or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently released, false otherwise
		 */
		bool IsMouseButtonUp(EMouseButton button) const noexcept;

		/**
		 * Indicate whether the scroll is down or not
		 * @return True if the scroll is down, false otherwise
		 */
		bool IsScrollDown() const noexcept;

		/**
		 * Indicate whether the scroll is up or not
		 * @return True if the scroll is up, false otherwise
		 */
		bool IsScrollUp() const noexcept;
	};

	inline EStateInput InputSystem::GetKeyState(EKey key) const noexcept
	{
		if (IsKeyRegistered(key))
			return m_registeredKeys.at(key);

		return EStateInput::UNUSED;
	}

	inline EStateInput InputSystem::GetMouseButtonState(EMouseButton button) const noexcept
	{
		if (IsMouseButtonRegistered(button))
			return m_registeredMouseButtons.at(button);

		return EStateInput::UNUSED;
	}

	inline EStateScroll InputSystem::GetScrollState() const noexcept
	{
		return m_registeredScrollYAxis;
	}

	inline void InputSystem::SetKeyDown(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateInput::PRESS;
	}

	inline void InputSystem::SetKeyUp(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateInput::UP;
	}

	inline void InputSystem::SetMouseButtonDown(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateInput::PRESS;
	}

	inline void InputSystem::SetMouseButtonUp(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateInput::UP;
	}

	inline void InputSystem::SetScrollYAxis(double yoffset) noexcept
	{
		if (yoffset < 0)
			m_registeredScrollYAxis = EStateScroll::DOWN;
		else if (yoffset > 0)
			m_registeredScrollYAxis = EStateScroll::UP;
	}

	inline void InputSystem::ClearRegisteredKeys() noexcept
	{
		for (auto itr = m_registeredKeys.begin(); itr != m_registeredKeys.end();)
		{
			if (itr->second == EStateInput::PRESS)
			{
				itr->second = EStateInput::DOWN;
				itr++;
			}
			else if (itr->second == EStateInput::UP)
				itr = m_registeredKeys.erase(itr);
			else
				++itr;
		}
	}

	inline void InputSystem::ClearRegisteredMouseButtons() noexcept
	{
		for (auto itr = m_registeredMouseButtons.begin(); itr != m_registeredMouseButtons.end();)
		{
			if (itr->second == EStateInput::PRESS)
			{
				itr->second = EStateInput::DOWN;
				itr++;
			}
			else if (itr->second == EStateInput::UP)
				itr = m_registeredMouseButtons.erase(itr);
			else
				++itr;
		}
	}

	inline void InputSystem::ClearRegisteredScroll() noexcept
	{
		if (m_registeredScrollYAxis != EStateScroll::UNUSED)
			m_registeredScrollYAxis = EStateScroll::UNUSED;
	}

	inline void InputSystem::ClearRegisteredInputs() noexcept
	{
		ClearRegisteredKeys();
		ClearRegisteredMouseButtons();
		ClearRegisteredScroll();
	}

	inline bool InputSystem::IsKeyRegistered(EKey key) const noexcept
	{
		return m_registeredKeys.find(key) != m_registeredKeys.end();
	}

	inline bool InputSystem::IsMouseButtonRegistered(EMouseButton button) const noexcept
	{
		return m_registeredMouseButtons.find(button) != m_registeredMouseButtons.end();
	}

	inline bool InputSystem::IsKeyPressed(EKey key) const noexcept
	{
		return IsKeyRegistered(key) && GetKeyState(key) == EStateInput::PRESS;
	}

	inline bool InputSystem::IsKeyDown(EKey key) const noexcept
	{
		return IsKeyRegistered(key) && GetKeyState(key) == EStateInput::DOWN || GetKeyState(key) == EStateInput::PRESS;
	}

	inline bool InputSystem::IsKeyUp(EKey key) const noexcept
	{
		return IsKeyRegistered(key) && GetKeyState(key) == EStateInput::UP;
	}

	inline bool InputSystem::IsMouseButtonPressed(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && GetMouseButtonState(button) == EStateInput::PRESS;
	}

	inline bool InputSystem::IsMouseButtonDown(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && GetMouseButtonState(button) == EStateInput::DOWN || GetMouseButtonState(button) == EStateInput::PRESS;
	}

	inline bool InputSystem::IsMouseButtonUp(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && GetMouseButtonState(button) == EStateInput::UP;
	}

	inline bool InputSystem::IsScrollDown() const noexcept
	{
		return  GetScrollState() == EStateScroll::DOWN;
	}

	inline bool InputSystem::IsScrollUp() const noexcept
	{
		return  GetScrollState() == EStateScroll::UP;
	}
}
