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

	void	error(int error, const char* message)
	{
		std::cout << message << std::endl;
	}

	int EditorEngine::Init(const int width, const int height)
	{
		glfwSetErrorCallback(error);
		int init = EngineCore::Init(width, height);
		if (init != 0)
			return init;
		if (!glfwInit())
			return 1;
		glfwMakeContextCurrent(m_window);
		if (!gladLoadGL())
			return 1;

		OnResizeWindow += BIND_EVENT_2(EditorEngine::SetSizeWindow);

		GLFWimage icons[1];
		icons[0].pixels = stbi_load("Resources\\Images\\bread.png", &icons[0].width, &icons[0].height, 0, STBI_rgb_alpha);
		glfwSetWindowIcon(m_window, 1, icons);
		stbi_image_free(icons[0].pixels);

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
		ImVec4	clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.f);
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
		glfwPollEvents();
		EngineCore::Update();
	}

	Core::Maths::Vec2 EditorEngine::GetMousePos() noexcept
	{
		return Core::Maths::Vec2();
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
}