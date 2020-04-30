#include "Canvas.hpp"

#include "EditorEngine.h"
#include "GUIManager.h"
#include "Separator.h"

#include "IconsFontAwesome5.h"

namespace Editor
{
	Canvas::Canvas(Editor::GUIManager* manager) :
		m_manager{ manager }
	{
		m_dockWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		m_dockWindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_dockWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		m_dockNodeFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;

		InitMenuBar();
	}

	Canvas::~Canvas()
	{
		delete m_menuBar;
	}

	void Canvas::InitMenuBar()
	{
		m_menuBar = new MenuBar();
		m_view = &m_menuBar->Add<Widget::MenuGroup>("View");
		m_view->Add<Widget::MenuItem>("Open All", "CTRL + P").OnClick += std::bind(&Canvas::SetAllWindowVisibility, this, true);
		m_view->Add<Widget::MenuItem>("Close All", "CTRL + M").OnClick += std::bind(&Canvas::SetAllWindowVisibility, this, false);
		m_view->Add<Widget::Separator>();
	}

	void Canvas::SetAllWindowVisibility(bool opened)
	{
		for (auto& widget : m_contents)
			widget->isVisible = opened;
	}

	bool Canvas::IsWindowFocused(const size_t id) const
	{
		return (GetFocusedWindow() == id) ? true : false;
	}

	size_t Canvas::GetFocusedWindow() const
	{
		int idFrame{ -1 };
		size_t id{ 0 };

		for (size_t i{ 0 }; i < m_contents.size(); ++i)
		{
			int currentIdFrame{ m_contents[i]->LastFrameFocused() };
			if (idFrame < currentIdFrame)
			{
				idFrame = currentIdFrame;
				id = i;
			}
		}

		return id;
	}

	void Canvas::CheckShortcuts()
	{
		Core::SystemManagement::InputSystem* inputSystem = GetEngine()->GetInputSystem();

		if (inputSystem->IsKeyDown(EKey::LEFT_CONTROL) && inputSystem->IsKeyPressed(EKey::P))
			SetAllWindowVisibility(true);
		if (inputSystem->IsKeyDown(EKey::LEFT_CONTROL) && inputSystem->IsKeyPressed(EKey::M))
			SetAllWindowVisibility(false);
	}

	void Canvas::SetViewport()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
	}

	void Canvas::BuildDockspace()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
		bool dockspace = ImGui::Begin("## Dockspace", NULL, m_dockWindowFlags);
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 4.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.f, 0.f, 0.f, 0.15f });
		if (ImGui::BeginChild("Toolbar", { 0.f, 34.f }))
		{
			DrawToolbar();
			ImGui::EndChild();
		}
		ImGui::Separator();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		if (dockspace)
		{
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.f, 0.f), m_dockNodeFlags);
			ImGui::End();
		}
	}

	void Canvas::SetDockspace()
	{
		SetViewport();
		BuildDockspace();
	}

	void Canvas::Draw()
	{
		CheckShortcuts();
		SetDockspace();
		m_menuBar->Draw();

		for (auto& window : m_contents)
		{
			window->Draw();
		}

#ifdef _DEBUG
		ImGui::ShowDemoWindow();
#endif
	}

	GUIManager* Canvas::GetManager() noexcept
	{
		return m_manager;
	}

	EditorEngine* Canvas::GetEngine() noexcept
	{
		return m_manager->GetEngine();
	}

	void Canvas::DrawToolbar()
	{
		ImGui::SetCursorPos({ 5.f, 5.f });
		if (ToolbarButton(ICON_FA_HAND_PAPER, GetEngine()->operation == SelectionMode::MOVEMENT, "Movement"))
			GetEngine()->operation = SelectionMode::MOVEMENT;
		ImGui::SameLine();
		if (ToolbarButton(ICON_FA_ARROWS_ALT, GetEngine()->operation == SelectionMode::TRANSLATION, "Translate"))
			GetEngine()->operation = SelectionMode::TRANSLATION;
		ImGui::SameLine();
		if (ToolbarButton(ICON_FA_SYNC_ALT, GetEngine()->operation == SelectionMode::ROTATION, "Rotate"))
			GetEngine()->operation = SelectionMode::ROTATION;
		ImGui::SameLine();
		if (ToolbarButton(ICON_FA_EXPAND, GetEngine()->operation == SelectionMode::SCALE, "Scale"))
			GetEngine()->operation = SelectionMode::SCALE;
		ImGui::SameLine(140.f);

		if (ToolbarButton(ICON_FA_MAP_MARKER_ALT "  Local", GetEngine()->gizmoMode == ImGuizmo::MODE::LOCAL, "Local"))
			GetEngine()->gizmoMode = ImGuizmo::MODE::LOCAL;
		ImGui::SameLine();

		if (ToolbarButton(ICON_FA_GLOBE "  World", GetEngine()->gizmoMode == ImGuizmo::MODE::WORLD, "World"))
			GetEngine()->gizmoMode = ImGuizmo::MODE::WORLD;
		ImGui::SameLine({ ImGui::GetWindowWidth() / 2 - 54.f });

		if (GetEngine()->IsPlaying())
		{
			if (ImGui::Button(ICON_FA_STOP))
				GetEngine()->EndPlay();
			ImGui::HelpMarkerItem("Stop");
		}
		else
		{
			if (ImGui::Button(ICON_FA_PLAY))
				GetEngine()->Play();
			ImGui::HelpMarkerItem("Play");
		}

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAUSE))
		ImGui::HelpMarkerItem("Pause");

		ImGui::SameLine();
		ImGui::Button(ICON_FA_STEP_FORWARD);
		ImGui::HelpMarkerItem("Next");

		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_COG))
		{
			GetEngine()->GetResourcesManager()->ReloadScripts();
			GetEngine()->GetResourcesManager()->ReloadShaders();
		}
		ImGui::HelpMarkerItem("Compile");
	}

	bool Canvas::ToolbarButton(const char* label, bool state, const char* helpMarker)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, state ? ImGui::GetStyle().Colors[ImGuiCol_Button] : ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
		bool isPressed = ImGui::Button(label);
		ImGui::HelpMarkerItem(helpMarker);	ImGui::SameLine();
		ImGui::PopStyleColor();
		return isPressed;
	}
}
