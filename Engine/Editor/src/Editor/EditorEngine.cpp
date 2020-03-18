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

		m_man = new Editor::GUIManager(m_window, glsl_version, Editor::GUIStyle::BAKER);

		Editor::Canvas* canvas = new Editor::Canvas();

		canvas->Add<Editor::Window::WindowHierarchy>();
		canvas->Add<Editor::Window::WindowInspector>();
		canvas->Add<Editor::Window::WindowScene>();
		canvas->Add<Editor::Window::WindowConsole>();
		canvas->Add<Editor::Window::WindowFileBrowser>();
		canvas->Add<Editor::Window::WindowProfiler>(false);

		m_man->SetCanvas(canvas);

		ImVec4	clear_color = ImVec4(0.45f, 0.55f, 0.6f, 1.f);

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
	}

	void EditorEngine::Update()
	{
	}

	Core::Maths::Vec2 EditorEngine::GetMousePos() noexcept
	{
		return Core::Maths::Vec2();
	}
}