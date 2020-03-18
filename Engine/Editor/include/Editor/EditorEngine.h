#pragma once

#include "EngineCore.h"
#include "GuiManager.h"
#include "CoreMinimal.h"
#include "LoadResources.h"

namespace Editor
{
	struct Framebuffer
	{
		GLuint FBO;
		GLuint ColorTexture;
		GLuint DepthStencilRenderbuffer;
		int	   Size[4];
	};
	class EditorEngine : public Core::Datastructure::EngineCore
	{
	protected:
		Editor::GUIManager* m_man;
		GLFWwindow* m_window;
		Framebuffer	m_fbo;
		TRACY_GL_IMAGE
	public:
		EditorEngine();
		EditorEngine(int width, int height);

		int		Init(const int width, const int height) override;
		void	SetSizeWindow(const double width, const double height) override;
		void	Update() override;

		virtual Core::Maths::Vec2	GetMousePos() noexcept override;
		GLFWwindow*					GetWindow();
		Framebuffer&				GetFBO();
		void						ResizeFBO(int width, int height);
	private:
		void				SetCallbackToGLFW();
		GLFWkeyfun			SetKeyCallBackToGLFW();
		GLFWmousebuttonfun	SetMouseButtonCallBackToGLFW();
		GLFWscrollfun		SetScrollCallBackToGLFW();
		GLFWwindowsizefun	SetWindowSizeToGLFW();

		Framebuffer			CreateFrameBuffer(int width, int height);
		void				DeleteFrameBuffer();
	};
}