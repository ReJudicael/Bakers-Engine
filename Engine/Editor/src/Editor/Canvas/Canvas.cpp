#include "Canvas.hpp"
#include <MenuSeparator.h>

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
		m_menuBar = new Menu::MenuBar();
		m_view = &m_menuBar->Add<Menu::MenuGroup>("View");
		m_view->Add<Menu::MenuItem>("Open All", "CTRL + P").OnClick += std::bind(&Canvas::OpenAllWidgets, this, true);
		m_view->Add<Menu::MenuItem>("Close All", "CTRL + M").OnClick += std::bind(&Canvas::OpenAllWidgets, this, false);
		m_view->Add<Menu::MenuSeparator>();
	}

	void Canvas::RemoveWidget(const Window::AWindow& widget)
	{
		auto found = std::find_if(m_widgets.begin(), m_widgets.end(),
			[&widget](std::unique_ptr<Window::AWindow>& w)
			{
				return w.get() == &widget;
			});

		if (found != m_widgets.end())
		{
			m_widgets.erase(found);
		}
	}

	void Canvas::RemoveAllWidgets()
	{
		m_widgets.clear();
	}

	void Canvas::OpenAllWidgets(bool opened)
	{
		for (auto& widget : m_widgets)
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

		for (auto& widget : m_widgets)
		{
			widget->Draw();
		}
	}
}
