#include "WindowScene.h"
#include "EditorEngine.h"
#include "ICamera.h"

#include <string>

namespace Editor::Window
{
	WindowScene::WindowScene(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Scene", visible }
	{
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		//Should be replaced by scene camera code
		m_fbo = GetEngine()->GetFBO(0);
	}

	void WindowScene::PushWindowStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	}

	void WindowScene::PopWindowStyle()
	{
		ImGui::PopStyleVar(1);
	}

	void WindowScene::DisplayScene()
	{
		if (m_fbo == nullptr)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("  No scene camera available");
		}
		else
		{
			ImVec2 windowSize{ ImGui::GetContentRegionAvail() };

			if (m_fbo->Size[2] != windowSize.x || m_fbo->Size[3] != windowSize.y)
			{
				//To redo with the scene camera;
				Core::Datastructure::ICamera* cam = reinterpret_cast<Core::Datastructure::ICamera*>(m_fbo->userPtr);
				if (cam != nullptr)
					cam->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
				else
					m_fbo->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
			}

#pragma warning(suppress : 4312)
			ImGui::ImageUV(m_fbo->ColorTexture, windowSize);
		}
	}

	void WindowScene::Tick()
	{
		DisplayScene();
	}
}
