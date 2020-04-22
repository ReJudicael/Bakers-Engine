#include "Canvas.hpp"

#include "EditorEngine.h"
#include "GUIManager.h"
#include "Separator.h"

namespace Editor
{
	Canvas::Canvas()
	{
		m_dockWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		m_dockWindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_dockWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		m_dockNodeFlags |= ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
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
		m_view->Add<Widget::MenuItem>("Save scene", "CTRL + S").OnClick += std::bind(&EditorEngine::SaveScene, GetEngine());
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
		if (inputSystem->IsKeyDown(EKey::LEFT_CONTROL) && inputSystem->IsKeyPressed(EKey::S))
			GetEngine()->SaveScene();
	}

	void Canvas::PushDockStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	}

	void Canvas::PopDockStyle()
	{
		ImGui::PopStyleVar(2);
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
		if (ImGui::Begin("## Dockspace", NULL, m_dockWindowFlags))
		{
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.f, 0.f), m_dockNodeFlags);
			ImGui::End();
		}
	}

	void Canvas::SetDockspace()
	{
		PushDockStyle();
		{
			SetViewport();
			BuildDockspace();
		}
		PopDockStyle();
	}

	void Canvas::Draw()
	{
		CheckShortcuts();
		SetDockspace();
		m_menuBar->Draw();

		for (auto& widget : m_contents)
		{
			widget->Draw();
		}

#ifdef _DEBUG
		ImGui::ShowDemoWindow();
#endif
	}

	GUIManager* Canvas::GetManager() noexcept
	{
		return m_manager;
	}

	void Canvas::SetManager(GUIManager* manager) noexcept
	{
		m_manager = manager;

		InitMenuBar();
	}

	EditorEngine* Canvas::GetEngine() noexcept
	{
		return m_manager->GetEngine();
	}
}
