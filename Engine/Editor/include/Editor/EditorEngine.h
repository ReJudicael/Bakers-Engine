#pragma once

#include "PxScene.h"

#include "EngineCore.h"
#include "GuiManager.h"
#include "CoreMinimal.h"
#include "FileSystem.hpp"
#include "InputSystem.hpp"
#include "ImGuizmo.h"
#include "Collider.h"

#include "json.hpp"
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
		Editor::GUIManager* m_man{ nullptr };

		bool				m_paused{ false };
		bool				m_step{ false };

		json				m_savedScene;

		std::list<Core::Renderer::Mesh*>	m_meshesNeedInit;


	public:
		Core::Datastructure::Object* objectSelected{ nullptr };
		std::shared_ptr<Resources::Material> materialSelected{ nullptr };
		std::string nameMaterialSelected = "";
		std::shared_ptr<Resources::Shader> shaderSelected;
		std::string nameShaderSelected;
		SelectionMode operation{ SelectionMode::TRANSLATION };
		ImGuizmo::MODE gizmoMode{ ImGuizmo::MODE::LOCAL };
		bool isTestingRay{ false };
		Core::Maths::Vec2 relativeMousePos;

		std::list<Core::Physics::Collider*>											m_boxCollider;
		std::list<Core::Physics::Collider*>											m_sphereCollider;
		std::list<Core::Physics::Collider*>											m_capsuleCollider;
		// The information of the Object3D loaded
		std::map<std::string, std::shared_ptr<Resources::Loader::Object3DInfo>>		m_info3DObject;

		Core::SystemManagement::InputSystem* m_editorInput{ nullptr };

		physx::PxScene*		m_editorScene;

	public:
		EditorEngine();
		EditorEngine(int width, int height);
		~EditorEngine();

		int		OnInit(const int width, const int height) override;
		void	SetSizeWindow(const int width, const int height) override;
		void	MainLoop();
		void	EndFrame() override;
		void	OnLoop() override;
		void	Render() override;
		bool	IsSelectingEngineView();
		void	SelectObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& direction);
		
		/**
		 * Init the mesh for create his editor box
		 * @param mesh: the lesh who need to be init
		 */
		virtual void InitMesh(Core::Renderer::Mesh* mesh) override;

		/**
		 * push the collider has a box collider
		 * for draw him
		 * @param coll: the collider
		 */
		virtual void PutBoxCollider(Core::Physics::Collider* coll) override;
		/**
		 * delete the collider from the list
		 * @param coll: the collider
		 */
		virtual void DeleteBoxCollider(Core::Physics::Collider* coll) override;
		/**
		 * push the collider has a sphere collider
		 * for draw him
		 * @param coll: the collider
		 */
		virtual void PutSphereCollider(Core::Physics::Collider* coll) override;
		/**
		 * delete the collider from the list
		 * @param coll: the collider
		 */
		virtual void DeleteSphereCollider(Core::Physics::Collider* coll) override;
		/**
		 * push the collider has a capsule collider
		 * for draw him
		 * @param coll: the collider
		 */
		virtual void PutCapsuleCollider(Core::Physics::Collider* coll) override;
		/**
		 * delete the collider from the list
		 * @param coll: the collider
		 */
		virtual void DeleteCapsuleCollider(Core::Physics::Collider* coll) override;

		/**
		 * put the Object3DInfo gived in the info3DObject map
		 * @param fileName: the name of the 3D Object
		 * @param Object3D: the Object3Info of the 3D object
		 */
		virtual void SaveObject3DInfo(const char* fileName, const Resources::Loader::Object3DInfo& Object3D) override;
		/**
		 * Get a Object3DInfo
		 * @param localPath: the local path of the 3D Object
		 * @return the Object3DInfo finded
		 */
		std::shared_ptr<Resources::Loader::Object3DInfo> GetObject3DInfo(const std::string& localPath)
		{
			if (m_info3DObject.count(localPath))
				return m_info3DObject[localPath];
			return nullptr;
		}

		virtual Core::Maths::Vec2	GetMousePos() noexcept override;
		virtual Core::Maths::Vec2	GetMousePosInWindow() noexcept override;

		void	Play() { m_state = Core::Datastructure::EngineState::STARTING; }
		void	EndPlay() { m_state = Core::Datastructure::EngineState::CLOSING; }
		bool	IsPlaying() { return m_state >= Core::Datastructure::EngineState::STARTING && m_state <= Core::Datastructure::EngineState::CLOSING; }
		void	Pause() { m_paused = true; }
		void	Unpause() { m_paused = false; }
		void	TogglePause() { m_paused = !m_paused; }
		bool	IsPaused() { return m_paused; }
		void	Step() { m_step = true; }
		void	UpdateSavedScene();
		void	SaveScene();
		void	ReloadScene();

		void	CompileNavMesh();

		void	SetGameInputState(bool state) { m_inputSystem->SetActive(state); };
	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();

		void UpdateMeshNeedToInit();
	};
}