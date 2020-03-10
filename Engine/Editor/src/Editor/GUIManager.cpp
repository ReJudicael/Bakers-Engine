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

	void GUIManager::SetStyleBaker()
	{
		ImGui::GetIO().Fonts->AddFontFromFileTTF("Resources\\Fonts\\Lato-Regular.ttf", 16)->DisplayOffset.y = -1;

		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowRounding					= 0.f;
		style->ChildRounding					= 0.f;
		style->FrameRounding					= 3.f;
		style->PopupRounding					= 3.f;
		style->ScrollbarRounding				= 3.f;
		style->GrabRounding						= 3.f;
		style->TabRounding						= 3.f;
		style->Colors[ImGuiCol_WindowBg]		= ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBg]			= ImVec4(0.25f, 0.25f, 0.30f, 0.50f);
		style->Colors[ImGuiCol_FrameBgHovered]	= ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive]	= ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive]	= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg]	= ImVec4(0.50f, 1.00f, 0.50f, 0.33f);
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
		case GUIStyle::BAKER:
			SetStyleBaker();
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
