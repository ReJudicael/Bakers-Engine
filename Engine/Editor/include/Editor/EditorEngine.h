#pragma once

#include "EngineCore.h"
#include "GuiManager.h"
#include "CoreMinimal.h"
#include "FileSystem.hpp"
#include "ImGuizmo.h"

namespace Editor
{
	class EditorEngine : public Core::Datastructure::EngineCore
	{
	protected:
		Editor::GUIManager* m_man{ nullptr };
	public:
		Core::Datastructure::Object* objectSelected{ nullptr };
		ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::LOCAL;

	public:
		EditorEngine();
		EditorEngine(int width, int height);
		~EditorEngine();

		int		OnInit(const int width, const int height) override;
		void	SetSizeWindow(const int width, const int height) override;
		void	MainLoop();
		void	EndFrame() override;
		void	OnLoop() override;
		bool	IsSelectingEngineView();

		virtual Core::Maths::Vec2	GetMousePos() noexcept override;

		void	Play() { m_state = Core::Datastructure::EngineState::STARTING; }
		void	EndPlay() { m_state = Core::Datastructure::EngineState::CLOSING; }
		bool	IsPlaying() { return m_state >= Core::Datastructure::EngineState::STARTING && m_state <= Core::Datastructure::EngineState::CLOSING; }
	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();
	};
}