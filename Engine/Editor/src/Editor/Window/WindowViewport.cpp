#include <string>

#include "WindowViewport.h"
#include "EditorEngine.h"
#include "ICamera.h"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowViewport::WindowViewport(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_GAMEPAD "  Viewport", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;
	}

	void WindowViewport::PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	}

	void WindowViewport::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowViewport::ChooseViewport()
	{
		ImGui::SetNextItemWidth(125.f);

		int camNum{ GetEngine()->GetFBONum(Core::Renderer::FBOType::CAMERA) };
		if (m_cameraNum >= camNum)
			m_cameraNum = camNum - 1;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.f, 4.f });
		if (camNum > 0 && ImGui::BeginCombo("## Camera", ("Viewport " + std::to_string(m_cameraNum)).c_str()))
		{
			for (int i{ 0 }; i < camNum; ++i)
			{
				if (ImGui::MenuItem(("Viewport " + std::to_string(i)).c_str()))
					m_cameraNum = i;
			}
			ImGui::EndCombo();
		}
		ImGui::PopStyleVar();
	}

	void WindowViewport::DisplayFBO(Core::Renderer::Framebuffer* fbo)
	{
		const ImVec2& windowSize{ ImGui::GetContentRegionAvail() };

		if (fbo->Size[2] != windowSize.x || fbo->Size[3] != windowSize.y)
		{
			Core::Datastructure::ICamera* cam = reinterpret_cast<Core::Datastructure::ICamera*>(fbo->userPtr);
			if (cam != nullptr)
				cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			else
				fbo->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
		}
		ImGui::ImageUV(fbo->ColorTexture, windowSize);
	}

	void WindowViewport::DisplayViewport()
	{
		Core::Renderer::Framebuffer* fbo{ GetEngine()->GetFBO(m_cameraNum, Core::Renderer::FBOType::CAMERA) };
		if (fbo)
		{
			float cursorY{ ImGui::GetCursorPosY() };
			DisplayFBO(fbo);

			ImGui::SetCursorPos({ ImGui::GetWindowContentRegionWidth() - 128.f, cursorY + GImGui->Style.ItemSpacing.y });
			ChooseViewport();
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No viewport available");
		}
	}

	void WindowViewport::Tick()
	{
		DisplayViewport();
		GetEngine()->SetGameInputState(ImGui::IsWindowHovered());
	}
}
