#pragma once

#include "EventSystem.hpp"
#include "InputSystem.hpp"
#include "CoreMinimal.h"
#include "Framebuffer.h"
#include "LoadResources.h"

namespace Core::Datastructure
{
	static const char* glsl_version = "#version 460";
	class RootObject;
	BAKERS_API_CLASS EngineCore
	{
	protected:
		Core::SystemManagement::InputSystem* m_inputSystem{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };
		std::list<Core::Renderer::Framebuffer*> m_fbo;
		GLFWwindow* m_window;
		Resources::Loader::ResourcesManager* m_manager;

		double m_time{ 0 };

		TRACY_GL_IMAGE
			
		double			GetDeltaTime();
	public:
		EngineCore();
		EngineCore(const int width, const int height);
		virtual ~EngineCore();

		Core::Datastructure::RootObject* m_root;

		Core::SystemManagement::EventSystem<EKey> OnPressKey;
		Core::SystemManagement::EventSystem<EKey> OnReleaseKey;
		Core::SystemManagement::EventSystem<EMouseButton> OnPressMouseButton;
		Core::SystemManagement::EventSystem<EMouseButton> OnReleaseMouseButton;
		Core::SystemManagement::EventSystem<double> OnScrollYAxis;
		Core::SystemManagement::EventSystem<double, double> OnResizeWindow;

		int				Init();
		virtual int		Init(const int width, const int height);
		virtual void	SetSizeWindow(const int width, const int height) = 0;

		virtual void	StartFrame();
		virtual void	Render();
		virtual void	EndFrame();

		virtual void	Update();

		GLFWwindow*		GetWindow();

		Core::Renderer::Framebuffer*	GetFBO(int num);
		Core::Renderer::Framebuffer*	CreateFBO();
		Core::Renderer::Framebuffer*	CreateFBO(int width, int height);
		void							DeleteFBO(Core::Renderer::Framebuffer* fbo);

		virtual Core::Maths::Vec2	GetMousePos() noexcept = 0;

		REGISTER_CLASS()
	};
}