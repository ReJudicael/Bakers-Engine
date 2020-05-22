#pragma once

#include "CoreMinimal.h"
#include "EventSystem.hpp"
#include "InputSystem.hpp"
#include "Framebuffer.h"
#include "LoadResources.h"
#include "NavMesh.h"

#include "json.hpp"
using nlohmann::json;

namespace Core
{
	namespace Audio
	{
		class AudioSystem;
	}
	namespace Physics
	{
		class PhysicsScene;
		class Collider;
	}
	namespace Datastructure
	{
		enum class EngineState
		{
			NONE = 0,
			INITIALIZING,
			INITIALIZED,
			STARTING,
			RUNNING,
			CLOSING,
			CLOSED,
		};
		
		static const char* glsl_version = "#version 400";
		class RootObject;
		BAKERS_API_CLASS EngineCore
		{
		protected:
			EngineState								m_state {EngineState::NONE};
			Core::SystemManagement::InputSystem *	m_inputSystem{ nullptr };
			Core::Audio::AudioSystem*				m_audioSystem{ nullptr };
			Core::SystemManagement::TaskSystem*		m_task{ nullptr };
			int m_width{ 0 };
			int m_height{ 0 };
			std::list<Core::Renderer::Framebuffer*>	m_fbo;
			GLFWwindow* m_window;
			Resources::Loader::ResourcesManager*	m_manager;

			Core::Physics::PhysicsScene*			m_physicsScene;

			Core::Navigation::NavMeshBuilder*		m_navMesh;

			std::string								m_currScene{ "Default" };

			double									m_time{ 0 };

			TRACY_GL_IMAGE

			double									GetDeltaTime();
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
			Core::SystemManagement::EventSystem<int, int> OnResizeWindow;

			inline Resources::Loader::ResourcesManager* GetResourcesManager()
			{
				return m_manager;
			}

			inline Core::Physics::PhysicsScene* GetPhysicsScene()
			{
				return m_physicsScene;
			}

			int				Init();
			int				Init(const int width, const int height);
			
			/**
			 * Create object with mesh and add it to hierarchy
			 * @param name: Name of the object to create
			 * @param model: Name of the model taken from Resources Manager
			 * @param shader: Name of the shader taken from Resources Manager
			 * @param tex: Path to the texture to load
			 * @param trs: Store position, rotation and scale to give to new object
			 * @param parent: Parent of the mesh object (default is root)
			 * @return Pointer to the created object
			 */
			Object* 	AddMesh(const char* name, 
								const char* model, 
								const char* shader, 
								const char* tex, 
								const Transform& trs,
								Object* parent = nullptr);

			/**
			 * Create skybox with a cube or a sphere
			 * @param cube: If true, create 6 quad to display sky, else, create one sphere with different texture
			 */
			void CreateSkybox(bool cube = true);

			virtual void	SetSizeWindow(const int width, const int height) = 0;

			virtual void	StartFrame();
			virtual void	Render();
			virtual void	EndFrame();

			void			Update(double deltaTime); 

			Object* SearchObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& dir);
			bool			LoadScene(const std::string& scene);
		protected:
			virtual int		OnInit(const int width, const int height);
			virtual void	OnLoop();
			virtual void	OnStartFrame();
			virtual void	OnUpdate(double deltaTime);
			bool			OnLoadScene();
			void			LoadSceneFromJson(json& scene);
		public:

			GLFWwindow* GetWindow();

			virtual void InitMesh(Core::Renderer::Mesh* mesh) {};

			virtual void PutBoxCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteBoxCollider(Core::Physics::Collider* mesh) {};
			virtual void PutSphereCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteSphereCollider(Core::Physics::Collider* mesh) {};
			virtual void PutCapsuleCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteCapsuleCollider(Core::Physics::Collider* mesh) {};

			virtual void SaveObject3DInfo(const char* fileName, const Resources::Loader::Object3DInfo& Object3D) {};

			virtual std::shared_ptr<Resources::Object3DGraph> LoadObject(const char* fileName, const bool graphInMulti = false);

			Core::SystemManagement::InputSystem* GetInputSystem();
			Core::Audio::AudioSystem* GetAudioSystem();
			int								GetFBONum(Core::Renderer::FBOType t);
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