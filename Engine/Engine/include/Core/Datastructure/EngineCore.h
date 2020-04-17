#pragma once

#include "EventSystem.hpp"
#include "InputSystem.hpp"
#include "CoreMinimal.h"
#include "Framebuffer.h"
#include "LoadResources.h"
#include "NavMesh.h"

namespace Core
{
	namespace Physics
	{
		class PhysicsScene;
	}
	namespace Datastructure
	{
		static const char* glsl_version = "#version 400";
		class RootObject;
		BAKERS_API_CLASS EngineCore
		{
		protected:
			Core::SystemManagement::InputSystem * m_inputSystem{ nullptr };
			int m_width{ 0 };
			int m_height{ 0 };
			std::list<Core::Renderer::Framebuffer*> m_fbo;
			GLFWwindow* m_window;
			Resources::Loader::ResourcesManager* m_manager;
			Core::Physics::PhysicsScene* m_physicsScene;

			Core::Navigation::NavMeshBuilder* m_navMesh;


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

			inline Resources::Loader::ResourcesManager* GetResourcesManager()
			{
				return m_manager;
			}

			inline Core::Physics::PhysicsScene* GetPhysicsScene()
			{
				return m_physicsScene;
			}

			int				Init();
			virtual int		Init(const int width, const int height);
			virtual void	SetSizeWindow(const int width, const int height) = 0;

			virtual void	StartFrame();
			virtual void	Render();
			virtual void	EndFrame();

			virtual void	Update(double deltaTime);

			void			DoLoop();

			GLFWwindow* GetWindow();

			Core::SystemManagement::InputSystem* GetInputSystem();
			Core::Renderer::Framebuffer*	GetFBO(int num, Core::Renderer::FBOType t = Core::Renderer::FBOType::CUSTOM);
			Core::Renderer::Framebuffer*	CreateFBO();
			Core::Renderer::Framebuffer*	CreateFBO(int width, int height, Core::Renderer::FBOType t = Core::Renderer::FBOType::CUSTOM);
			void							DeleteFBO(Core::Renderer::Framebuffer* fbo);

			virtual Core::Maths::Vec2		GetMousePos() noexcept = 0;

			void							AddMeshToNav(Vertex* verts, int nverts, GLuint* tris, int ntris, const Core::Datastructure::Transform& position);
			Navigation::NavMeshBuilder*		GetNavMesh() { return m_navMesh; }

		private:
			void				SetCallbackToGLFW();
			GLFWkeyfun			SetKeyCallBackToGLFW();
			GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
			GLFWscrollfun		SetScrollCallBackToGLFW();
			GLFWwindowsizefun	SetWindowSizeToGLFW();

			REGISTER_CLASS()
		};
	}
}