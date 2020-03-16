#include "Canvas.hpp"

namespace Editor
{
	Canvas::Canvas()
	{
		m_dockFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		m_dockFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_dockFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	}

	void Canvas::RemoveWidget(std::string nameID)
	{
		for (size_t i{ 0 }; i < m_widgets.size();)
		{
			if (m_widgets[i].first->GetNameID().compare(nameID) == 0)
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
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

	void Canvas::BuildDockWidgets(ImGuiID dock_main_id)
	{
		using U16 = unsigned short int;
		ImGuiID	anchor[(U16)EAnchor::COUNT];

		anchor[(U16)EAnchor::MIDDLE]		= dock_main_id;
		anchor[(U16)EAnchor::LEFT]			= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Left);
		anchor[(U16)EAnchor::RIGHT]			= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Right);
		anchor[(U16)EAnchor::TOP]			= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Up);
		anchor[(U16)EAnchor::BOTTOM]		= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Down);

		anchor[(U16)EAnchor::MIDDLE_LEFT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Left);
		anchor[(U16)EAnchor::MIDDLE_RIGHT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Right);
		anchor[(U16)EAnchor::MIDDLE_TOP]	= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Up);
		anchor[(U16)EAnchor::MIDDLE_BOTTOM]	= DockBuilderSplitNode(anchor[(U16)EAnchor::MIDDLE], ImGuiDir_Down);

		anchor[(U16)EAnchor::TOP_LEFT]		= DockBuilderSplitNode(anchor[(U16)EAnchor::TOP], ImGuiDir_Left);
		anchor[(U16)EAnchor::TOP_RIGHT]		= DockBuilderSplitNode(anchor[(U16)EAnchor::TOP], ImGuiDir_Right);
		anchor[(U16)EAnchor::TOP_TOP]		= DockBuilderSplitNode(anchor[(U16)EAnchor::TOP], ImGuiDir_Up);
		anchor[(U16)EAnchor::TOP_BOTTOM]	= DockBuilderSplitNode(anchor[(U16)EAnchor::TOP], ImGuiDir_Down);

		anchor[(U16)EAnchor::BOTTOM_LEFT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::BOTTOM], ImGuiDir_Left);
		anchor[(U16)EAnchor::BOTTOM_RIGHT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::BOTTOM], ImGuiDir_Right);
		anchor[(U16)EAnchor::BOTTOM_TOP]	= DockBuilderSplitNode(anchor[(U16)EAnchor::BOTTOM], ImGuiDir_Up);
		anchor[(U16)EAnchor::BOTTOM_BOTTOM]	= DockBuilderSplitNode(anchor[(U16)EAnchor::BOTTOM], ImGuiDir_Down);

		anchor[(U16)EAnchor::LEFT_LEFT]		= DockBuilderSplitNode(anchor[(U16)EAnchor::LEFT], ImGuiDir_Left);
		anchor[(U16)EAnchor::LEFT_RIGHT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::LEFT], ImGuiDir_Right);
		anchor[(U16)EAnchor::LEFT_TOP]		= DockBuilderSplitNode(anchor[(U16)EAnchor::LEFT], ImGuiDir_Up);
		anchor[(U16)EAnchor::LEFT_BOTTOM]	= DockBuilderSplitNode(anchor[(U16)EAnchor::LEFT], ImGuiDir_Down);

		anchor[(U16)EAnchor::RIGHT_LEFT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::RIGHT], ImGuiDir_Left);
		anchor[(U16)EAnchor::RIGHT_RIGHT]	= DockBuilderSplitNode(anchor[(U16)EAnchor::RIGHT], ImGuiDir_Right);
		anchor[(U16)EAnchor::RIGHT_TOP]		= DockBuilderSplitNode(anchor[(U16)EAnchor::RIGHT], ImGuiDir_Up);
		anchor[(U16)EAnchor::RIGHT_BOTTOM]	= DockBuilderSplitNode(anchor[(U16)EAnchor::RIGHT], ImGuiDir_Down);

		for (std::pair<std::unique_ptr<Widget::IWidget>, EAnchor>& dockWidget : m_widgets)
		{
			ImGui::DockBuilderDockWindow(dockWidget.first->GetNameID().c_str(), anchor[(U16)dockWidget.second]);
		}

		ImGui::DockBuilderFinish(dock_main_id);
	}

	void Canvas::BuildDockspace()
	{
		if (ImGui::Begin("## Dockspace", NULL, m_dockFlags))
		{
			const ImGuiID dockspace_id = ImGui::GetID("dockspace_id");
			if (!ImGui::DockBuilderGetNode(dockspace_id))
			{
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

				ImGuiID dock_main_id = dockspace_id;
				BuildDockWidgets(dock_main_id);
			}
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
					if (ImGui::Selectable(widget.first->GetName().c_str()))
					{
						widget.first->SetVisible(true);
						ImGui::SetWindowFocus(widget.first->GetNameID().c_str());
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
			widget.first->Draw();
		}
	}

	ImGuiID Canvas::DockBuilderSplitNode(ImGuiID& ID, ImGuiDir_ dir)
	{
		return ImGui::DockBuilderSplitNode(ID, dir, 0.3f, nullptr, &ID);
	}
}
