#pragma once

#include "CoreMinimal.h"
#include "EventSystem.hpp"
#include "InputSystem.hpp"
#include "Framebuffer.h"
#include "ResourcesManager.h"
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
		
		/**
		 * Core of the engine. Contains everything necessary to run the engine
		 */
		BAKERS_API_CLASS EngineCore
		{
		protected:
			EngineState		m_state {EngineState::NONE};
			GLFWwindow*		m_window{ nullptr };
			int				m_width{ 0 };
			int				m_height{ 0 };

			double			m_time{ 0 };
			std::string		m_currScene{ ".\\Resources\\Scenes\\Default.bakers" };
			std::list<Core::Renderer::Framebuffer*>	m_fbo;

			TRACY_GL_IMAGE

			Core::SystemManagement::InputSystem *	m_inputSystem{ nullptr };
			Core::Audio::AudioSystem*				m_audioSystem{ nullptr };
			Core::SystemManagement::TaskSystem*		m_task{ nullptr };
			Resources::Loader::ResourcesManager*	m_manager{ nullptr };
			Core::Physics::PhysicsScene*			m_physicsScene{ nullptr };
			Core::Navigation::NavMeshBuilder*		m_navMesh{ nullptr };

		public:
			Core::Datastructure::RootObject* m_root{ nullptr };

			Core::SystemManagement::EventSystem<EKey> OnPressKey;
			Core::SystemManagement::EventSystem<EKey> OnReleaseKey;
			Core::SystemManagement::EventSystem<EMouseButton> OnPressMouseButton;
			Core::SystemManagement::EventSystem<EMouseButton> OnReleaseMouseButton;
			Core::SystemManagement::EventSystem<double> OnScrollYAxis;
			Core::SystemManagement::EventSystem<int, int> OnResizeWindow;

		protected:
			/**
			 * Returns the computed deltaTime of the frame. To call once per frame internally
			 */
			double GetDeltaTime();

		public:
			/**
			 * Constructor
			 */
			EngineCore();

			/**
			 * Value constructor
			 * @param width
			 * @param height
			 */
			EngineCore(const int width, const int height);

			/**
			 * Destructor
			 */
			virtual ~EngineCore();

			/**
			 * Sets the size of the window internally. Will not actually change the size of the window
			 */
			virtual void		SetSizeWindow(const int width, const int height) = 0;
		public:


			/**
			 * Calls Init with default width and height as parameters
			 */
			int	Init();

			/**
			 * Initializes engine with given width and height for the window. 
			 * Initializes opengl and loads default scene
			 */
			int	Init(const int width, const int height);
			
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
			Object* AddMesh(const char* name, 
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

			/**
			 * Calls all the function that must be called at the start of a frame
			 */
			virtual void		StartFrame();
			/**
			 * Calls all the function that must be called to render the engine
			 */
			virtual void		Render();
			/**
			 * Calls all the function that must be called at the end of a frame
			 */
			virtual void		EndFrame();

			/**
			 * Calls all functions necessary to update a frame of the engine
			 * @param deltaTime: detaTime of the update
			 */
			void				Update(double deltaTime); 

			Object*				SearchObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& dir);

			/**
			 * Loads the scene given as path
			 * @param scene: Scene to load
			 */
			bool				LoadScene(const std::string& scene);
			/**
			 * Gets the name of the current scene
			 */
			const std::string&	GetSceneName() const { return m_currScene; }

		protected:
			/**
			 * Called in Init(), must be overriden to implement window
			 */
			virtual int			OnInit(const int width, const int height);
			/**
			 * Called for each frame
			 */
			virtual void		OnLoop();
			/**
			 * Called in StartFrame. Must be overriden to call all user code that needs to be called at the start of a frame
			 */
			virtual void		OnStartFrame();
			/**
			 * Called in OnUpdate. Must be overriden to call all user code that needs to be called in an update of a frame
			 */
			virtual void		OnUpdate(double deltaTime);
			/**
			 * Loads the current scene. If the name has not changed, reloads the scene
			 */
			bool				OnLoadScene();
			/**
			 * Extracts scene data from given json scene and loads it in the scene
			 * @param scene: Json scene to be loaded
			 */
			void				LoadSceneFromJson(json& scene);

		public:
			GLFWwindow*			GetWindow();

			virtual void InitMesh(Core::Renderer::Mesh* mesh) {};

			virtual void PutBoxCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteBoxCollider(Core::Physics::Collider* mesh) {};
			virtual void PutSphereCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteSphereCollider(Core::Physics::Collider* mesh) {};
			virtual void PutCapsuleCollider(Core::Physics::Collider* mesh) {};
			virtual void DeleteCapsuleCollider(Core::Physics::Collider* mesh) {};

			virtual void SaveObject3DInfo(const char* fileName, const Resources::Loader::Object3DInfo& Object3D) {};

			virtual std::shared_ptr<Resources::Object3DGraph> LoadObject(const char* fileName, const bool graphInMulti = false);

			Resources::Loader::ResourcesManager*	GetResourcesManager();
			Core::Physics::PhysicsScene*			GetPhysicsScene();
			Core::SystemManagement::InputSystem*	GetInputSystem();
			Core::Audio::AudioSystem*				GetAudioSystem();

			/**
			 * Returns the number of fbo object of the given type
			 */
			int								GetFBONum(Core::Renderer::FBOType t);
			/**
			 * Retuns the fbo object of the given type and num
			 * @param num: Number of the fbo object
			 * @param t: Type of the fbo object
			 * @return requested fbo object or nullptr if it does not exists
			 */
			Core::Renderer::Framebuffer*	GetFBO(int num, Core::Renderer::FBOType t = Core::Renderer::FBOType::CUSTOM);
			/**
			 * Creates default fbo object of current window size and type custom
			 */
			Core::Renderer::Framebuffer*	CreateFBO();
			/**
			 * Creates a fbo object of given size and given type
			 * @param width: width of the fbo
			 * @param width: height of the fbo
			 * @param t: type of the fbo
			 */
			Core::Renderer::Framebuffer*	CreateFBO(int width, int height, Core::Renderer::FBOType t = Core::Renderer::FBOType::CUSTOM);
			/**
			 * Deletes the given FBO object
			 */
			void							DeleteFBO(Core::Renderer::Framebuffer* fbo);

			virtual Core::Maths::Vec2		GetMousePos() noexcept = 0;
			virtual Core::Maths::Vec2		GetMousePosInWindow() noexcept = 0;

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