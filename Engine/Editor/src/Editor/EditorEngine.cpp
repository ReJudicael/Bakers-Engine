#include "EditorEngine.h"
#include "GUIManager.h"
#include "stb_image.h"

#include "WindowFileBrowser.h"
#include "WindowConsole.h"
#include "WindowInspector.h"
#include "WindowScene.h"
#include "WindowHierarchy.h"
#include "WindowProfiler.h"
#include "MenuGroup.h"
#include "RootObject.hpp"
#include "CoreMinimal.h"
#include "LoadResources.h"

namespace Editor
{
	EditorEngine::EditorEngine() : EditorEngine(1280, 800)
	{
	}

	EditorEngine::EditorEngine(int width, int height) : EngineCore(width, height)
	{
		Init(width, height);
	}

	int EditorEngine::Init(const int width, const int height)
	{
		if (!glfwInit())
			return 1;

		const char* glsl_version = "#version 460";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		m_window = glfwCreateWindow(width, height, "Editor", nullptr, nullptr);
		if (m_window == nullptr)
			return 1;

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		if (!gladLoadGL())
		{
			fprintf(stderr, "gladLoadGL failed. \n");
			return 1;
		}
		TracyGpuContext

		OnResizeWindow += BIND_EVENT_2(EditorEngine::SetSizeWindow);

		GLFWimage icons[1];
		icons[0].pixels = stbi_load("Resources\\Images\\bread.png", &icons[0].width, &icons[0].height, 0, STBI_rgb_alpha);
		glfwSetWindowIcon(m_window, 1, icons);
		stbi_image_free(icons[0].pixels);

		SetCallbackToGLFW();

		m_man = new Editor::GUIManager(this, glsl_version, Editor::GUIStyle::BAKER);

		Editor::Canvas* canvas = new Editor::Canvas();
		m_man->SetCanvas(canvas);

		canvas->Add<Editor::Window::WindowHierarchy>(this);
		canvas->Add<Editor::Window::WindowInspector>(this);
		canvas->Add<Editor::Window::WindowViewport>(this);
		canvas->Add<Editor::Window::WindowConsole>(this);
		canvas->Add<Editor::Window::WindowFileBrowser>(this);
		canvas->Add<Editor::Window::WindowProfiler>(this, false);


		ImVec4	clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.f);
		m_fbo = CreateFrameBuffer(m_width, m_height);
		INIT_TRACY_GL_IMAGE(m_width, m_height)
		// Main loop
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			// Start the Dear ImGui frame

			// Rendering
			Update();
			m_man->Render();

			int display_w, display_h;
			glfwGetFramebufferSize(m_window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glfwSwapBuffers(m_window);
		}


		glfwDestroyWindow(m_window);
		glfwTerminate();
		return 0;
	}

	void EditorEngine::SetSizeWindow(const double width, const double height)
	{
		m_width = width;
		m_height = height;
	}

	void EditorEngine::Update()
	{
		ZoneNamedN(updateLoop, "Main update loop", true)
			glfwPollEvents();
		m_root->StartFrame();
		double deltaTime = GetDeltaTime();
		m_root->Update(deltaTime);
		GLint PreviousFramebuffer;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		m_root->Render();
		{
			ZoneNamedN(swapBuffers, "glfwSwapBuffers", true)
			TRACY_GL_IMAGE_SEND(m_width, m_height)
			TracyGpuCollect
		}
		glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);
		FrameMark;
		m_root->RemoveDestroyed();
		m_inputSystem->ClearRegisteredInputs();
	}

	Core::Maths::Vec2 EditorEngine::GetMousePos() noexcept
	{
		return Core::Maths::Vec2();
	}

	GLFWwindow* EditorEngine::GetWindow()
	{
		return m_window;
	}

	Framebuffer& EditorEngine::GetFBO()
	{
		return m_fbo;
	}

	void EditorEngine::ResizeFBO(int width, int height)
	{

		glBindTexture(GL_TEXTURE_2D, m_fbo.ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		m_fbo.Size[2] = width;
		m_fbo.Size[3] = height;
	}


	void EditorEngine::SetCallbackToGLFW()
	{
		SetKeyCallBackToGLFW();
		SetMouseButtonCallBackToGLFW();
		SetScrollCallBackToGLFW();
		SetWindowSizeToGLFW();
	}

	GLFWkeyfun EditorEngine::SetKeyCallBackToGLFW()
	{
		GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ZoneScopedN("InputSystemKeyUpdate")
				ZoneText("Updating key presses", 21)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				if (action == GLFW_PRESS)
					this_window->OnPressKey(static_cast<EKey>(key));
				if (action == GLFW_RELEASE)
					this_window->OnReleaseKey(static_cast<EKey>(key));
			}
		};
		return glfwSetKeyCallback(m_window, key_callback);
	}

	GLFWmousebuttonfun EditorEngine::SetMouseButtonCallBackToGLFW()
	{
		GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			ZoneScopedN("InputSystemMouseButtonUpdate")
				ZoneText("Updating MouseButton presses", 29)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				if (action == GLFW_PRESS)
					this_window->OnPressMouseButton(static_cast<EMouseButton>(button));
				else if (action == GLFW_RELEASE)
					this_window->OnReleaseMouseButton(static_cast<EMouseButton>(button));
			}
		};
		return glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	}

	GLFWscrollfun EditorEngine::SetScrollCallBackToGLFW()
	{
		GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			ZoneScopedN("InputSystemScrollUpdate")
				ZoneText("Updating Scrolling", 19)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnScrollYAxis(yoffset);
			}
		};
		return glfwSetScrollCallback(m_window, scroll_callback);
	}

	GLFWwindowsizefun EditorEngine::SetWindowSizeToGLFW()
	{
		GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
		{
			EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnResizeWindow(width, height);
			}
		};
		return glfwSetWindowSizeCallback(m_window, window_size_callback);
	}

	Framebuffer EditorEngine::CreateFrameBuffer(int width, int height)
	{
		GLint PreviousFramebuffer;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);

		// Create Framebuffer that will hold 1 color attachement
		GLuint FBO;
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Create texture that will be used as color attachment
		GLuint ColorTexture;
		glGenTextures(1, &ColorTexture);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glObjectLabel(GL_TEXTURE, ColorTexture, -1, "ColorTexture");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Note: Here we store the depth stencil in a renderbuffer object, 
		// but we can as well store it in a texture if we want to display it later
		GLuint DepthStencilRenderbuffer;
		glGenRenderbuffers(1, &DepthStencilRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
		glObjectLabel(GL_RENDERBUFFER, DepthStencilRenderbuffer, -1, "DepthStencilRenderbuffer");
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		// Setup attachements
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, ColorTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRenderbuffer);

		unsigned int DrawAttachments[1] = { GL_COLOR_ATTACHMENT0 + 0 };
		glDrawBuffers(1, DrawAttachments);

		GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "demo_impostor::framebuffer failed to complete (0x%x)\n", FramebufferStatus);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);

		Framebuffer Framebuffer = {};
		Framebuffer.FBO = FBO;
		Framebuffer.ColorTexture = ColorTexture;
		Framebuffer.DepthStencilRenderbuffer = DepthStencilRenderbuffer;
		Framebuffer.Size[0] = 0;
		Framebuffer.Size[1] = 0;
		Framebuffer.Size[2] = width;
		Framebuffer.Size[3] = height;

		return Framebuffer;
	}

	void EditorEngine::DeleteFrameBuffer()
	{
		glDeleteTextures(1, &m_fbo.ColorTexture);
		glDeleteFramebuffers(1, &m_fbo.FBO);
	}

}