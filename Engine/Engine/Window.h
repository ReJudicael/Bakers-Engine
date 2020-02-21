#pragma once

#include <GLFW/glfw3.h>

#include "EventSystem.hpp"

class Window
{
private:
	GLFWwindow* m_window = nullptr;

public:
	Core::SystemManagement::EventSystem<EKey> OnPressKey;
	Core::SystemManagement::EventSystem<EKey> OnReleaseKey;
	Core::SystemManagement::EventSystem<EMouseButton> OnPressMouseButton;
	Core::SystemManagement::EventSystem<EMouseButton> OnReleaseMouseButton;

	Window();
	Window(const int height, const int width);
	~Window();

	void	Update();
};