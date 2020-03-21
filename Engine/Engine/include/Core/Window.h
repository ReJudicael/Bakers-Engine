#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputSystem.hpp"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "CoreMinimal.h"

class Window : public Core::Datastructure::EngineCore
{
public:
	Window();
	Window(const int width, const int height);
	~Window();

	int		Init(const int width, const int height) override;
	void	SetSizeWindow(const int width, const int height) override;
	void	Update() override;

	virtual Core::Maths::Vec2	GetMousePos() noexcept override;

	GLFWwindow* GetGLFWwindow() { return m_window; }
private:
	GLFWwindow*			m_window{ nullptr };
	void				SetCallbackToGLFW();
	GLFWkeyfun			SetKeyCallBackToGLFW();
	GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
	GLFWscrollfun		SetScrollCallBackToGLFW();
	GLFWwindowsizefun	SetWindowSizeToGLFW();
};
