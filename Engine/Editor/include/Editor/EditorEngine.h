#pragma once

#include "PxScene.h"
#include <PxScene.h>
#include <json.hpp>

#include "EngineCore.h"
#include "GuiManager.h"
#include "CoreMinimal.h"
#include "InputSystem.hpp"
#include "ImGuizmo.h"
#include "Collider.h"

using nlohmann::json;

namespace Editor
{
	enum class SelectionMode
	{
		TRANSLATION = ImGuizmo::OPERATION::TRANSLATE,
		ROTATION = ImGuizmo::OPERATION::ROTATE,
		SCALE = ImGuizmo::OPERATION::SCALE,
		MOVEMENT
	};

	class EditorEngine : public Core::Datastructure::EngineCore
	{
	protected:
		bool m_paused{ false };
		bool m_step{ false };

		json m_savedScene;

		Editor::GUIManager* m_man{ nullptr };
		std::list<Core::Renderer::Mesh*> m_meshesNeedInit;

	public:
		#pragma region ItemSelected
		Core::Datastructure::Object* objectSelected{ nullptr };

		std::shared_ptr<Resources::Material> materialSelected{ nullptr };
		std::string nameMaterialSelected;

		std::shared_ptr<Resources::Shader> shaderSelected;
		std::string nameShaderSelected;
		#pragma endregion

		SelectionMode operation{ SelectionMode::TRANSLATION };
		ImGuizmo::MODE gizmoMode{ ImGuizmo::MODE::LOCAL };

		bool isTestingRay{ false };
		Core::Maths::Vec2 relativeMousePos;

		std::list<Core::Physics::Collider*> m_boxCollider;
		std::list<Core::Physics::Collider*> m_sphereCollider;
		std::list<Core::Physics::Collider*> m_capsuleCollider;
		std::map<std::string, std::shared_ptr<Resources::Loader::Object3DInfo>> m_info3DObject;
		physx::PxScene* m_editorScene;

		Core::SystemManagement::InputSystem* m_editorInput{ nullptr };

	public:
		/**
		 * Default constructor
		 */
		EditorEngine();

		/**
		 * Value constructor
		 * @param width: Width of window
		 * @param height: Height of window
		 */
		EditorEngine(int width, int height);

		/**
		 * Destructor
		 */
		~EditorEngine();

	public:
		/**
		 * Init Editor (glfwContext, GUIManager, Canvas)
		 * @param width: Width of window
		 * @param height: Height of window
		 */
		int OnInit(const int width, const int height) override;

		/**
		 * Set size window
		 * @param width: Next widht of window
		 * @param height: Next height of window
		 */
		void SetSizeWindow(const int width, const int height) override;

		/**
		 * Main loop (call OnLoop)
		 */
		void MainLoop();

		/**
		 * Called at the end of each frame
		 */
		void EndFrame() override;

		/**
		 * On Loop
		 */
		void OnLoop() override;

		/**
		 * Render FBO and the canvas of the GUIManager
		 */
		void Render() override;

		/**
		 * Indicate whether the engine view (Viewport window) is focused or not
		 * @return True if the engine view is focused, false otherwise
		 */
		bool IsSelectingEngineView();

		/**
		 * Select a gameObject in Scene with a raycast
		 * @param origin: Origin of ray
		 * @param direction: Direction of ray
		 */
		void SelectObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& direction);
		
		/**
		 * Init the mesh for create his editor box
		 * @param mesh: the lesh who need to be init
		 */
		virtual void InitMesh(Core::Renderer::Mesh* mesh) override;

		/**
		 * Push the collider has a box collider for draw him
		 * @param coll: Collider
		 */
		virtual void PutBoxCollider(Core::Physics::Collider* coll) override;

		/**
		 * Delete the collider from the list
		 * @param coll: Collider
		 */
		virtual void DeleteBoxCollider(Core::Physics::Collider* coll) override;

		/**
		 * Push the collider has a sphere collider for draw him
		 * @param coll: Collider
		 */
		virtual void PutSphereCollider(Core::Physics::Collider* coll) override;

		/**
		 * Delete the collider from the list
		 * @param coll: Collider
		 */
		virtual void DeleteSphereCollider(Core::Physics::Collider* coll) override;

		/**
		 * push the collider has a capsule collider for draw him
		 * @param coll: Collider
		 */
		virtual void PutCapsuleCollider(Core::Physics::Collider* coll) override;

		/**
		 * Delete the collider from the list
		 * @param coll: Collider
		 */
		virtual void DeleteCapsuleCollider(Core::Physics::Collider* coll) override;

		/**
		 * Put the Object3DInfo gived in the info3DObject map
		 * @param fileName: Name of the 3D Object
		 * @param Object3D: Object3Info of the 3D object
		 */
		virtual void SaveObject3DInfo(const char* fileName, const Resources::Loader::Object3DInfo& Object3D) override;

		/**
		 * Get a Object3DInfo
		 * @param localPath: the local path of the 3D Object
		 * @return the Object3DInfo finded
		 */
		std::shared_ptr<Resources::Loader::Object3DInfo> GetObject3DInfo(const std::string& localPath);

		/**
		 * Get Mouse Position
		 */
		virtual Core::Maths::Vec2 GetMousePos() noexcept override;

		/**
		 * Get relative mouse position in window
		 */
		virtual Core::Maths::Vec2 GetMousePosInWindow() noexcept override;

	public:
		/**
		 * Play
		 * Set Engine state to "starting"
		 */
		void Play();

		/**
		 * End Play
		 * Set Engine state to "closing"
		 */
		void EndPlay();

		/**
		 * Wheter the editor is playing (playmode) or not
		 * @return True if the editor is playing, false otherwise
		 */
		bool IsPlaying();

		/**
		 * Set pausemode
		 */
		void Pause();

		/**
		 * Unpause
		 */
		void Unpause();

		/**
		 * Set pausemode or not
		 */
		void TogglePause();

		/**
		 * Wheter the editor is pausing (pausemode) or not
		 * @return True if the editor is paused, false otherwise
		 */
		bool IsPaused();

		/**
		 * Set stepmode
		 */
		void Step();

		/**
		 * Update saved scene
		 */
		void UpdateSavedScene();

		/**
		 * Save scene
		 */
		void SaveScene();

		/**
		 * Reload scene
		 */
		void ReloadScene();

		/**
		 * Compile Nav Mesh
		 */
		void CompileNavMesh();

		/**
		 * Set Game Input State
		 * @param state: Whether the user can use game input or not
		 */
		void SetGameInputState(bool state);

		/**
		 * Update mesh need to init
		 */
		void UpdateMeshNeedToInit();

	private:
		/**
		 * Set callback to glfw
		 */
		void SetCallbackToGLFW();
		/**
		 * Set keycallback to glfw
		 */
		GLFWkeyfun SetKeyCallBackToGLFW();

		/**
		 * Set mouse button callback to glfw
		 */
		GLFWmousebuttonfun SetMouseButtonCallBackToGLFW();

		/**
		 * Set scroll callback to glfw
		 */
		GLFWscrollfun SetScrollCallBackToGLFW();

		/**
		 * Set window size to glfw
		 */
		GLFWwindowsizefun SetWindowSizeToGLFW();
	};
}
