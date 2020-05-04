#include <iostream>
#include "GUIManager.h"
#include "EditorEngine.h"
#include "ImGuizmo.h"
#include "IconsFontAwesome5.h"

#include <ImGuizmo.h>

namespace Editor
{
	GUIManager::GUIManager(EditorEngine* engine, const char* glsl_version, GUIStyle style) :
		m_engine {engine}
	{
#if _DEBUG
		IMGUI_CHECKVERSION();
#endif

		ImGui::CreateContext();
		{
			SetIOConfiguration();
			SetFonts();
			SetSizes();
			SetStyle(style);
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(engine->GetWindow(), true);
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

	void GUIManager::SetFonts()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Lato-Regular.ttf", 16.f)->DisplayOffset.y = -1;

		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.f; // Use if you want to make the icon monospaced
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\" FONT_ICON_FILE_NAME_FAR, 14.f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\" FONT_ICON_FILE_NAME_FAS, 14.f, &config, icon_ranges);
		io.Fonts->Build();
	}

	void GUIManager::SetSizes()
	{
		ImGuiStyle* style				= &ImGui::GetStyle();
		style->WindowMenuButtonPosition	= ImGuiDir_Right;
		style->WindowRounding			= 0.f;
		style->ChildRounding			= 0.f;
		style->FrameRounding			= 3.f;
		style->PopupRounding			= 3.f;
		style->ScrollbarRounding		= 3.f;
		style->GrabRounding				= 3.f;
		style->TabRounding				= 3.f;
		style->FramePadding				= { 4.f, 4.f };
		style->ItemSpacing				= { 4.f, 4.f };
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
		case GUIStyle::BAKER_DARK:
			ImGui::StyleColorsBakerDark();
			return true;
		case GUIStyle::BAKER_LIGHT:
			ImGui::StyleColorsBakerLight();
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
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
	}

	void GUIManager::EndFrame()
	{
		ImGui::EndFrame();
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

	bool GUIManager::IsWindowFocused(const size_t id) 
	{ 
		return m_canvas->IsWindowFocused(id); 
	}

	EditorEngine* GUIManager::GetEngine() noexcept
	{
		return m_engine;
	}
}
