#pragma once

#include "EngineCore.h"
#include "GuiManager.h"
#include "CoreMinimal.h"

namespace Editor
{
	class EditorEngine : public Core::Datastructure::EngineCore
	{
	protected:
		Editor::GUIManager* m_man;

	public:
		EditorEngine();
		EditorEngine(int width, int height);
		~EditorEngine();

		int		Init(const int width, const int height) override;
		void	SetSizeWindow(const double width, const double height) override;
		void	Update() override;

		virtual Core::Maths::Vec2	GetMousePos() noexcept override;

	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();
	};
}