#include "Canvas.hpp"
#include "GUIManager.h"
#include "Separator.h"

namespace Editor
{
	Canvas::Canvas()
	{
		m_dockWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		m_dockWindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_dockWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		m_dockNodeFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;

		 InitMenuBar();
	}

	void Canvas::InitMenuBar()
	{
		m_menuBar = new MenuBar();
		m_view = &m_menuBar->Add<Widget::MenuGroup>("View");
		m_view->Add<Widget::MenuItem>("Open All", "CTRL + P").OnClick += std::bind(&Canvas::OpenAllWindows, this, true);
		m_view->Add<Widget::MenuItem>("Close All", "CTRL + M").OnClick += std::bind(&Canvas::OpenAllWindows, this, false);
		m_view->Add<Widget::Separator>();
	}


	void Canvas::OpenAllWindows(bool opened)
	{
		for (auto& widget : m_contents)
			widget->isVisible = opened;
	}

	void Canvas::PushDockStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	}

	void Canvas::PopDockStyle()
	{
		ImGui::PopStyleVar(3);
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
			ImGuiID dockspace_id = ImGui::GetID("Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), m_dockNodeFlags);
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
		SetDockspace();
		m_menuBar->Draw();

		for (auto& widget : m_contents)
		{
			widget->Draw();
		}

		ImGui::ShowDemoWindow();
	}

	GUIManager* Canvas::GetManager() noexcept
	{
		return m_manager;
	}

	void Canvas::SetManager(GUIManager* manager) noexcept
	{
		m_manager = manager;
	}

	EditorEngine* Canvas::GetEngine() noexcept
	{
		return m_manager->GetEngine();
	}
}
