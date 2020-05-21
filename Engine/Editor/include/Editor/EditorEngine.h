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

		std::vector<Mesh*>	m_meshesNeedInit;


	public:
		Core::Datastructure::Object* objectSelected{ nullptr };
		std::shared_ptr<Resources::Material> materialSelected{ nullptr };
		SelectionMode operation{ SelectionMode::TRANSLATION };
		ImGuizmo::MODE gizmoMode{ ImGuizmo::MODE::WORLD };
		bool isTestingRay{ false };

		std::list<Core::Physics::Collider*> m_BoxCollider;
		std::list<Core::Physics::Collider*> m_SphereCollider;
		std::list<Core::Physics::Collider*> m_CapsuleCollider;

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
		
		virtual void InitMesh(Mesh* mesh) override;

		virtual void PutBoxCollider(Core::Physics::Collider* coll) override;
		virtual void DeleteBoxCollider(Core::Physics::Collider* coll) override;
		virtual void PutSphereCollider(Core::Physics::Collider* coll) override;
		virtual void DeleteSphereCollider(Core::Physics::Collider* coll) override;
		virtual void PutCapsuleCollider(Core::Physics::Collider* coll) override;
		virtual void DeleteCapsuleCollider(Core::Physics::Collider* coll) override;

		virtual Core::Maths::Vec2	GetMousePos() noexcept override;

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

		void	SetGameInputState(bool state) { m_inputSystem->SetActive(state); };
	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();
	};
}