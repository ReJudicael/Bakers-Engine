#pragma once

#include <chrono>
#include <iostream>

#include "EventSystem.hpp"
#include "Input.hpp"
#include "CoreMinimal.h"
#include "Vec2.hpp"

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
		std::unordered_map<EKey, EStateKey> m_registeredKeys;
		std::unordered_map<EMouseButton, EStateMouseButton> m_registeredMouseButtons;
		EStateScroll m_registeredScrollYAxis{ EStateScroll::UNUSED };

		/* Double click handling */
		std::chrono::time_point<std::chrono::system_clock> m_nowClock;
		std::chrono::time_point<std::chrono::system_clock> m_beforeClock;
		EMouseButton m_lastClickedButton{ EMouseButton::LEFT };
		bool m_isTimeSet = false;

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
		EStateKey GetKeyState(EKey key) const noexcept;

		/**
		 * Get the state (pressed or released) of the mouse button
		 * @param button: Button of which the state will be checked
		 * @return State of the given button
		 */
		EStateMouseButton GetMouseButtonState(EMouseButton button) const noexcept;

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
		 * Check whether the given button has been clicked on twice or not
		 * @param mouseButton: Button to be checked
		 */
		void CheckDoubleClick(EMouseButton button) noexcept;

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
		 * Indicate whether the given key is unused or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is currently unused, false otherwise
		 */
		bool IsKeyUnused(EKey key) const noexcept;

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
		 * Indicate whether the given mouse button has been clicked on twice or not
		 * @param button: Button of which the state will be checked
		 */
		bool IsMouseDoubleClicked(EMouseButton button) const noexcept;

		/**
		 * Indicate whether the given mouse button is unused or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently unused, false otherwise
		 */
		bool IsMouseButtonUnused(EMouseButton button) const noexcept;

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

		/**
		 * Indicate whether the scroll is unused or not
		 * @return True if the scroll is unused, false otherwise
		 */
		bool IsScrollUnused() const noexcept;

		/**
		 * Get cursor position
		 * @return The position of the cursor, in screen coordinates, relative to the upper-left corner of the content area of the window
		 */
		Core::Maths::Vec2 GetMousePos() const noexcept;
	};

	inline EStateKey InputSystem::GetKeyState(EKey key) const noexcept
	{
		if (IsKeyRegistered(key))
			return m_registeredKeys.at(key);

		return EStateKey::UNUSED;
	}

	inline EStateMouseButton InputSystem::GetMouseButtonState(EMouseButton button) const noexcept
	{
		if (IsMouseButtonRegistered(button))
			return m_registeredMouseButtons.at(button);

		return EStateMouseButton::UNUSED;
	}

	inline EStateScroll InputSystem::GetScrollState() const noexcept
	{
		return m_registeredScrollYAxis;
	}

	inline void InputSystem::SetKeyDown(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateKey::PRESS;
	}

	inline void InputSystem::SetKeyUp(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateKey::UP;
	}

	inline void InputSystem::SetMouseButtonDown(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateMouseButton::PRESS;
	}

	inline void InputSystem::SetMouseButtonUp(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateMouseButton::UP;
		CheckDoubleClick(button);
	}

	inline void InputSystem::CheckDoubleClick(EMouseButton button) noexcept
	{
		if (m_isTimeSet)
		{
			m_beforeClock = m_nowClock;
			m_nowClock = std::chrono::system_clock::now();
			double diff_ms = std::chrono::duration<double, std::milli>(m_nowClock - m_beforeClock).count();
			if (diff_ms > 10 && diff_ms < 500)
			{
				if (button == m_lastClickedButton)
				{
					m_registeredMouseButtons[button] = EStateMouseButton::DOUBLECLICK;
					m_isTimeSet = false;
				}
			}
		}
		else
		{
			m_nowClock = std::chrono::system_clock::now();
			m_isTimeSet = true;
		}
		m_lastClickedButton = button;
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
			if (itr->second == EStateKey::PRESS)
			{
				itr->second = EStateKey::DOWN;
				itr++;
			}
			else if (itr->second == EStateKey::UP)
				itr = m_registeredKeys.erase(itr);
			else
				++itr;
		}
	}

	inline void InputSystem::ClearRegisteredMouseButtons() noexcept
	{
		for (auto itr = m_registeredMouseButtons.begin(); itr != m_registeredMouseButtons.end();)
		{
			if (itr->second == EStateMouseButton::PRESS)
			{
				itr->second = EStateMouseButton::DOWN;
				itr++;
			}
			else if (itr->second == EStateMouseButton::UP || itr->second == EStateMouseButton::DOUBLECLICK)
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
		ZoneScoped
			ZoneText("Clearing inputs from InputSystem", 33)
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
		return IsKeyRegistered(key) && GetKeyState(key) == EStateKey::PRESS;
	}

	inline bool InputSystem::IsKeyDown(EKey key) const noexcept
	{
		return IsKeyRegistered(key) && (GetKeyState(key) == EStateKey::DOWN ||
			GetKeyState(key) == EStateKey::PRESS);
	}

	inline bool InputSystem::IsKeyUp(EKey key) const noexcept
	{
		return IsKeyRegistered(key) && GetKeyState(key) == EStateKey::UP;
	}

	inline bool InputSystem::IsKeyUnused(EKey key) const noexcept
	{
		return !IsKeyRegistered(key);
	}

	inline bool InputSystem::IsMouseButtonPressed(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && GetMouseButtonState(button) == EStateMouseButton::PRESS;
	}

	inline bool InputSystem::IsMouseButtonDown(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && (GetMouseButtonState(button) == EStateMouseButton::DOWN || 
			GetMouseButtonState(button) == EStateMouseButton::PRESS);
	}

	inline bool InputSystem::IsMouseButtonUp(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && (GetMouseButtonState(button) == EStateMouseButton::UP ||
			GetMouseButtonState(button) == EStateMouseButton::DOUBLECLICK);
	}

	inline bool InputSystem::IsMouseDoubleClicked(EMouseButton button) const noexcept
	{
		return IsMouseButtonRegistered(button) && GetMouseButtonState(button) == EStateMouseButton::DOUBLECLICK;
	}

	inline bool InputSystem::IsMouseButtonUnused(EMouseButton button) const noexcept
	{
		return !IsMouseButtonRegistered(button);
	}

	inline bool InputSystem::IsScrollDown() const noexcept
	{
		return  GetScrollState() == EStateScroll::DOWN;
	}

	inline bool InputSystem::IsScrollUp() const noexcept
	{
		return  GetScrollState() == EStateScroll::UP;
	}

	inline bool InputSystem::IsScrollUnused() const noexcept
	{
		return GetScrollState() == EStateScroll::UNUSED;
	}
}
