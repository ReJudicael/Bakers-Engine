#pragma once

#include <GLFW/glfw3.h>

#include "InputSystem.hpp"

class Window
{
private:
	GLFWwindow* m_window{ nullptr };
	Core::SystemManagement::InputSystem* m_inputSystem{ nullptr };

public:
	Core::SystemManagement::EventSystem<EKey> OnPressKey;
	Core::SystemManagement::EventSystem<EKey> OnReleaseKey;
	Core::SystemManagement::EventSystem<EMouseButton> OnPressMouseButton;
	Core::SystemManagement::EventSystem<EMouseButton> OnReleaseMouseButton;

	Window();
	Window(const int height, const int width);
	~Window();

	void	Init(const int height, const int width);
	void	Update();

	void				SetCallbackToGLFW();
	GLFWkeyfun			SetKeyCallBackToGLFW();
	GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
	GLFWscrollfun		SetScrollCallBackToGLFW();
};
