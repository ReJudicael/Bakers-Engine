#include "WindowToolbar.h"
#include "EditorEngine.h"

namespace Editor::Window
{
	WindowToolbar::WindowToolbar(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Toolbar", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\hand.png", m_guizmoIcons[0]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\move.png", m_guizmoIcons[1]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\rotate.png", m_guizmoIcons[2]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\scale.png", m_guizmoIcons[3]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\world.png", m_guizmoIcons[4]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\local.png", m_guizmoIcons[5]);

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\play.png", m_simulationIcons[0]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\stop.png", m_simulationIcons[1]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\pause.png", m_simulationIcons[2]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\next.png", m_simulationIcons[3]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\compile.png", m_simulationIcons[4]);
	}

	void WindowToolbar::PushWindowStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.f, 0.f, 0.f, 0.7f });
	}

	void WindowToolbar::PopWindowStyle()
	{
		ImGui::PopStyleColor();
	}

	void WindowToolbar::DisplayToolbar()
	{
		if (ImGui::ImageButtonUV(m_guizmoIcons[0]->texture))
			GetEngine()->operation = SelectionMode::MOVEMENT;
		ImGui::HelpMarkerItem("Movement");	ImGui::SameLine();
		
		if (ImGui::ImageButtonUV(m_guizmoIcons[1]->texture))
			GetEngine()->operation = SelectionMode::TRANSLATION;
		ImGui::HelpMarkerItem("Translate");	ImGui::SameLine();
		
		if (ImGui::ImageButtonUV(m_guizmoIcons[2]->texture))
			GetEngine()->operation = SelectionMode::ROTATION;
		ImGui::HelpMarkerItem("Rotate");	ImGui::SameLine();

		if (ImGui::ImageButtonUV(m_guizmoIcons[3]->texture))
			GetEngine()->operation = SelectionMode::SCALE;
		ImGui::HelpMarkerItem("Scale");		ImGui::SameLine(140.f);

		if (ImGui::ImageButtonUVWithText(m_guizmoIcons[4]->texture, "World"))
			GetEngine()->gizmoMode = ImGuizmo::MODE::LOCAL;
		ImGui::HelpMarkerItem("World");		ImGui::SameLine();

		if (ImGui::ImageButtonUVWithText(m_guizmoIcons[5]->texture, "Local"))
			GetEngine()->gizmoMode = ImGuizmo::MODE::WORLD;
		ImGui::HelpMarkerItem("Local");		ImGui::SameLine();

		ImGui::SetCursorPosX({ ImGui::GetWindowWidth() / 2 - 54.f });
		if (GetEngine()->IsPlaying())
		{
			if (ImGui::ImageButtonUV(m_simulationIcons[1]->texture))
				GetEngine()->EndPlay();
			ImGui::HelpMarkerItem("Stop");
		}
		else
		{
			if (ImGui::ImageButtonUV(m_simulationIcons[0]->texture))
				GetEngine()->Play();
			ImGui::HelpMarkerItem("Play");
		}

		ImGui::SameLine();
		ImGui::ImageButtonUV(m_simulationIcons[2]->texture);
		ImGui::HelpMarkerItem("Pause");

		ImGui::SameLine();
		ImGui::ImageButtonUV(m_simulationIcons[3]->texture);
		ImGui::HelpMarkerItem("Next");

		ImGui::SameLine();
		if (ImGui::ImageButtonUV(m_simulationIcons[4]->texture))
		{
			GetEngine()->GetResourcesManager()->ReloadScripts();
			GetEngine()->GetResourcesManager()->ReloadShaders();
		}
		ImGui::HelpMarkerItem("Compile");
	}

	void WindowToolbar::Tick()
	{
		DisplayToolbar();
	}
}
