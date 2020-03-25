#include "EditorEngine.h"
#include "GUIManager.h"
#include "stb_image.h"

#include "WindowFileBrowser.h"
#include "WindowConsole.h"
#include "WindowInspector.h"
#include "WindowViewport.h"
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

	EditorEngine::~EditorEngine()
	{
		delete m_man;
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
	void callback_error(int error, const char* message)
	{
		std::cerr << message << std::endl;
	}

	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	int EditorEngine::Init(const int width, const int height)
	{
		glfwSetErrorCallback(callback_error);

		if (EngineCore::Init(width, height))
			return 1;

		if (!glfwInit())
			return 1;

		if (!gladLoadGL())
			return 1;

		glDebugMessageCallback(MessageCallback, 0);

		glfwMakeContextCurrent(m_window);
		{
			GLFWimage icons[1];
			icons[0].pixels = stbi_load("Resources\\Images\\bread.png", &icons[0].width, &icons[0].height, 0, STBI_rgb_alpha);
			glfwSetWindowIcon(m_window, 1, icons);
			stbi_image_free(icons[0].pixels);
		}
		SetCallbackToGLFW();

		m_man = new Editor::GUIManager(this, Core::Datastructure::glsl_version, Editor::GUIStyle::BAKER);
		Editor::Canvas* canvas = new Editor::Canvas();
		m_man->SetCanvas(canvas);

		canvas->Add<Editor::Window::WindowHierarchy>();
		canvas->Add<Editor::Window::WindowInspector>();
		canvas->Add<Editor::Window::WindowViewport>();
		canvas->Add<Editor::Window::WindowConsole>();
		canvas->Add<Editor::Window::WindowFileBrowser>();
		canvas->Add<Editor::Window::WindowProfiler>(false);

		m_root->AddComponent(new Core::Datastructure::ComponentBase());
		INIT_TRACY_GL_IMAGE(m_width, m_height)

		return 0;
	}

	void EditorEngine::SetSizeWindow(const int width, const int height)
	{
		m_width = width;
		m_height = height;
	}

	void EditorEngine::Update()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glClearColor(0.45f, 0.55f, 0.6f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();
			EngineCore::Update();
			m_man->Render();

			int display_w, display_h;
			glfwGetFramebufferSize(m_window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);

			glfwSwapBuffers(m_window);
		}
	}

	Core::Maths::Vec2 EditorEngine::GetMousePos() noexcept
	{
		double pos[2];
		glfwGetCursorPos(GetWindow(), &pos[0], &pos[1]);
		return Core::Maths::Vec2(pos[0], pos[1]);
	}

	void EditorEngine::SetCallbackToGLFW()
	{
		OnResizeWindow += BIND_EVENT_2(EditorEngine::SetSizeWindow);

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
}