#include "WindowToolbar.h"
#include "EditorEngine.h"

namespace Editor::Window
{
	WindowToolbar::WindowToolbar(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Toolbar", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\play.png", m_icons[0]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\pause.png", m_icons[1]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\next.png", m_icons[2]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\reload.png", m_icons[3]);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\ToolbarIcons\\compilation.png", m_icons[4]);
	}

	void WindowToolbar::PushWindowStyle()
	{
	}

	void WindowToolbar::PopWindowStyle()
	{
	}

	void WindowToolbar::DisplayToolbar()
	{
		ImGui::ImageButtonUV_HelpMarker(m_icons[0]->texture, "Play", { 32.f });
		ImGui::SameLine();
		ImGui::ImageButtonUV_HelpMarker(m_icons[1]->texture, "Pause", { 32.f });
		ImGui::SameLine();
		ImGui::ImageButtonUV_HelpMarker(m_icons[2]->texture, "Next", { 32.f });
		ImGui::SameLine();
		ImGui::ImageButtonUV_HelpMarker(m_icons[3]->texture, "Reload", { 32.f });
		ImGui::SameLine();
		ImGui::ImageButtonUV_HelpMarker(m_icons[4]->texture, "Compilation", { 32.f });
	}

	void WindowToolbar::Tick()
	{
		DisplayToolbar();
	}
}
