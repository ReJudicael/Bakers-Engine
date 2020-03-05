#include "Canvas.h"

void Canvas::SetDockspace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags;
	window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	if (ImGui::Begin("## Dockspace", NULL, window_flags))
	{
		const ImGuiID dockspace_id = ImGui::GetID("dockspace_id");
		if (!ImGui::DockBuilderGetNode(dockspace_id))
		{
			// Reset current docking state
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
			ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

			ImGuiID dock_main_id = dockspace_id;
			ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.40f, nullptr, &dock_main_id);
			ImGuiID dock_id_right_down = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.6f, nullptr, &dock_id_right);
			ImGuiID dock_id_down_right = ImGui::DockBuilderSplitNode(dock_id_bottom, ImGuiDir_Right, 0.6f, nullptr, &dock_id_bottom);

			ImGui::DockBuilderDockWindow(W_SCENE, dock_main_id);
			ImGui::DockBuilderDockWindow(W_HIERARCHY, dock_id_left);
			ImGui::DockBuilderDockWindow(W_INSPECTOR, dock_id_right);
			ImGui::DockBuilderDockWindow(W_FILEBROWSER, dock_id_bottom);
			ImGui::DockBuilderDockWindow(W_CONSOLE, dock_id_bottom);
			ImGui::DockBuilderFinish(dock_main_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
	}
}
