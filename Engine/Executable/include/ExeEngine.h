#pragma once

#include "EngineCore.h"
#include "Vec2.hpp"

namespace Executable
{
	class ExeEngine : public Core::Datastructure::EngineCore
	{
	public:
		ExeEngine();
		ExeEngine(int width, int height);
		~ExeEngine();

		int		OnInit(const int width, const int height) override;
		void	MainLoop();
		void	SetSizeWindow(const int width, const int height) override;
		virtual Core::Maths::Vec2	GetMousePos() noexcept override;
		virtual Core::Maths::Vec2		GetMousePosInWindow() noexcept { return GetMousePos(); }
		void	OnLoop() override;

	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();

		GLuint				m_VAO{ 0 };
	};
}