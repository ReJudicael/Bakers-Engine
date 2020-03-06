#include "GUIManager.h"
#include "WidgetFileBrowser.h"
#include <iostream>

namespace Editor
{
	GUIManager::GUIManager(GLFWwindow* glfwWindow, const char* glsl_version, GUIStyle style)
	{
#if _DEBUG
		IMGUI_CHECKVERSION();
#endif

		ImGui::CreateContext();
		{
			SetIOConfiguration();
			SetStyle(style);
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	GUIManager::~GUIManager()
	{
		delete m_canvas;

		// Clean up
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();
	}

	void GUIManager::SetIOConfiguration()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	bool GUIManager::SetStyle(GUIStyle style)
	{
		switch (style)
		{
		case GUIStyle::CLASSIC:
			ImGui::StyleColorsClassic();
			return true;
		case GUIStyle::DARK:
			ImGui::StyleColorsDark();
			return true;
		case GUIStyle::LIGHT:
			ImGui::StyleColorsLight();
			return true;
		default:
			std::cout << "This style is not valid!" << std::endl;
			return false;
		}
	}

	void GUIManager::SetCanvas(Canvas* canvas)
	{
		m_canvas = canvas;
	}

	void GUIManager::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GUIManager::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void GUIManager::Render()
	{
		NewFrame();
		{
			if (m_canvas)
				m_canvas->Draw();
		}
		EndFrame();
	}
}
