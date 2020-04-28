#pragma once

#include "EngineCore.h"
#include "Vec2.hpp"

namespace Executable
{
	class ExeEngine : public Core::Datastructure::EngineCore
	{
	public:
		~ExeEngine();

		int		OnInit(const int width, const int height) override;
		void	MainLoop();
		void	SetSizeWindow(const int width, const int height) override;
		virtual Core::Maths::Vec2	GetMousePos() noexcept override;
		void	OnLoop() override;

	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();
	};
}