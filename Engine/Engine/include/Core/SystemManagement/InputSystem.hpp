#pragma once

#include "EventSystem.hpp"
#include "Input.hpp"
#include "Window.h"

namespace Core::SystemManagement
{
	/**
	 * Handle inputs by storing whether each key is pressed or released
	 */
	class InputSystem final
	{
	private:
		/* Stockage mouse buttons and keys */
		std::unordered_map<EKey, EStateInput> m_registeredKeys;
		std::unordered_map<EMouseButton, EStateInput> m_registeredMouseButtons;

		/* Events */
		ID m_keyPressedListenerID{ 0 };
		ID m_keyReleasedListenerID{ 0 };
		ID m_mouseButtonPressedListenerID{ 0 };
		ID m_mouseButtonReleasedListenerID{ 0 };

		Window* m_window;
		//EventSystem<int> OnPressed;
		//w.OnPressed += std::bind(IsKeyPressed, this, std::placeholders_1)

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
		 * Indicate whether the given key is pressed or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is currently pressed, false otherwise
		 */
		bool IsKeyDown(EKey key) const noexcept;
		/**
		 * Indicate whether the given key is pressed or not
		 * @param key: Key of which the state will be checked
		 * @return True if the key is currently released, false otherwise
		 */
		bool IsKeyUp(EKey key) const noexcept;
		/**
		 * Indicate whether the given mouse button is pressed or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently pressed, false otherwise
		 */
		bool IsMouseButtonDown(EMouseButton button) const noexcept;
		/**
		 * Indicate whether the given mouse button is pressed or not
		 * @param button: Button of which the state will be checked
		 * @return True if the button is currently released, false otherwise
		 */
		bool IsMouseButtonUp(EMouseButton button) const noexcept;
	};

	inline EStateInput InputSystem::GetKeyState(EKey key) const noexcept
	{
		for (const auto& [Key, State] : m_registeredKeys)
			if (Key == key)
				return State;
	}

	inline EStateInput InputSystem::GetMouseButtonState(EMouseButton button) const noexcept
	{
		for (const auto& [MouseButton, State] : m_registeredMouseButtons)
			if (MouseButton == button)
				return State;
	}

	inline void InputSystem::SetKeyDown(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateInput::DOWN;
	}

	inline void InputSystem::SetKeyUp(EKey key) noexcept
	{
		m_registeredKeys[key] = EStateInput::UP;
	}

	inline void InputSystem::SetMouseButtonDown(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateInput::DOWN;
	}

	inline void InputSystem::SetMouseButtonUp(EMouseButton button) noexcept
	{
		m_registeredMouseButtons[button] = EStateInput::UP;
	}

	inline bool InputSystem::IsKeyDown(EKey key) const noexcept
	{
		return GetKeyState(key) == EStateInput::DOWN;
	}

	inline bool InputSystem::IsKeyUp(EKey key) const noexcept
	{
		return GetKeyState(key) == EStateInput::UP;
	}

	inline bool InputSystem::IsMouseButtonDown(EMouseButton button) const noexcept
	{
		return GetMouseButtonState(button) == EStateInput::DOWN;
	}

	inline bool InputSystem::IsMouseButtonUp(EMouseButton button) const noexcept
	{
		return GetMouseButtonState(button) == EStateInput::UP;
	}
}