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
		if (ImGui::ImageButtonUV(m_icons[0]->texture, { 32.f }))
		{
			if (!GetEngine()->IsPlaying())
				GetEngine()->Play();
			else
				GetEngine()->EndPlay();
		}
		ImGui::HelpMarkerItem("Play");

		ImGui::SameLine();
		ImGui::ImageButtonUV(m_icons[1]->texture, { 32.f });
		ImGui::HelpMarkerItem("Pause");

		ImGui::SameLine();
		ImGui::ImageButtonUV(m_icons[2]->texture, { 32.f });
		ImGui::HelpMarkerItem("Next");

		ImGui::SameLine();
		ImGui::ImageButtonUV(m_icons[3]->texture, { 32.f });
		ImGui::HelpMarkerItem("Reload");

		ImGui::SameLine();
		if (ImGui::ImageButtonUV(m_icons[4]->texture, { 32.f }))
		{
			GetEngine()->GetResourcesManager()->ReloadScripts();
			GetEngine()->GetResourcesManager()->ReloadShaders();
		}
		ImGui::HelpMarkerItem("Compilation");
	}

	void WindowToolbar::Tick()
	{
		DisplayToolbar();
	}
}
