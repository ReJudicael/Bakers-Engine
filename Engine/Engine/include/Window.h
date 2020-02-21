#pragma once

#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* m_window = nullptr;
public:
	Window();
	Window(const int height, const int width);
	~Window();

	void	Update();
};