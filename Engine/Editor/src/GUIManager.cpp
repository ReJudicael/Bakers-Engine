#include "GUIManager.h"
#include <iostream>

Core::GUIManager::GUIManager(GLFWwindow* glfwWindow, const char* glsl_version, GUIStyle style)
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

Core::GUIManager::~GUIManager()
{
	// Clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
}

void Core::GUIManager::SetIOConfiguration()
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}

bool Core::GUIManager::SetStyle(GUIStyle style)
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

void Core::GUIManager::Render()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
