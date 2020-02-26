#pragma once

#include <GLFW/glfw3.h>

#include "InputSystem.hpp"
#include "RootObject.hpp"

class Window
{
private:
	GLFWwindow* m_window{ nullptr };
	Core::SystemManagement::InputSystem* m_inputSystem{ nullptr };
	int m_width{ 0 };
	int m_height{ 0 };

public:
	Core::SystemManagement::EventSystem<EKey> OnPressKey;
	Core::SystemManagement::EventSystem<EKey> OnReleaseKey;
	Core::SystemManagement::EventSystem<EMouseButton> OnPressMouseButton;
	Core::SystemManagement::EventSystem<EMouseButton> OnReleaseMouseButton;
	Core::SystemManagement::EventSystem<double> OnScrollYAxis;
	Core::SystemManagement::EventSystem<double, double> OnResizeWindow;

	Core::Datastructure::RootObject* m_root;

	Window();
	Window(const int height, const int width);
	~Window();

	void	Init(const int width, const int height);
	void	SetSizeWindow(const double width, const double height);
	void	Update();

	GLFWwindow* GetGLFWwindow() { return m_window; }

private:
	void				SetCallbackToGLFW();
	GLFWkeyfun			SetKeyCallBackToGLFW();
	GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
	GLFWscrollfun		SetScrollCallBackToGLFW();
	GLFWwindowsizefun	SetWindowSizeToGLFW();
};
