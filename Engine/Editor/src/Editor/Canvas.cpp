#include "Canvas.hpp"

namespace Editor
{
	Canvas::Canvas()
	{
		m_dockFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		m_dockFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_dockFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	void Canvas::RemoveWidget(std::string nameID)
	{
		for (size_t i{ 0 }; i < m_widgets.size();)
		{
			if (m_widgets[i]->GetNameID().compare(nameID) == 0)
				m_widgets.erase(m_widgets.begin() + i);
			else
				++i;
		}
	}

	void Canvas::RemoveAllWidgets()
	{
		m_widgets.clear();
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
		if (ImGui::Begin("## Dockspace", NULL, m_dockFlags))
		{
			ImGuiID dockspace_id = ImGui::GetID("Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_PassthruCentralNode);
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

	void Canvas::SetMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{

			if (ImGui::BeginMenu("View"))
			{

				int i = 0;
				for (const auto& widget : m_widgets)
				{
					ImGui::PushID(++i);
					if (ImGui::MenuItem(widget->GetNameID().c_str(), "", &widget->isVisible, true))
					{
						if (widget->isVisible)
							ImGui::SetWindowFocus(widget->GetNameID().c_str());
					}
					ImGui::PopID();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void Canvas::Draw()
	{
		SetDockspace();
		SetMenuBar();
		for (auto& widget : m_widgets)
		{
			widget->Draw();
		}
	}
}
