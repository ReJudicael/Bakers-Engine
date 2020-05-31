#include <glad\glad.h>

#include "WindowShader.h"
#include "EditorEngine.h"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowShader::WindowShader(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_ADJUST "  Shader", visible }
	{
		m_treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\InspectorIcons\\Material.png", m_propertyIcon);
	}

	void WindowShader::PushWindowStyle()
	{
	}

	void WindowShader::PopWindowStyle()
	{
	}

	void WindowShader::DragDropTargetItem(const std::string& itemPath, std::shared_ptr<Resources::Texture>& texture)
	{
		ImGui::RButtonDD("Texture",
			!itemPath.empty() ? (ICON_FA_FILE "  " + std::filesystem::path(itemPath).filename().string()).c_str() : "");
		if (!itemPath.empty())
			ImGui::HelpMarkerItem(itemPath.c_str());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
			{
				const std::string& path{ reinterpret_cast<const char*>(payload->Data) };
				GetEngine()->GetResourcesManager()->LoadTexture(path, texture);
			}
			ImGui::EndDragDropTarget();
		}
	}

	void WindowShader::ShaderInspector(std::shared_ptr<Resources::Shader>& shader)
	{
		if (ImGui::CollapsingHeaderWithImageUV(m_propertyIcon->texture, "Properties", m_treeNodeFlags))
		{
			ImGui::Spacing();

			DisplayVertAndFragShader(shader, true);

			DisplayVertAndFragShader(shader, false);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
			ImGui::SetNextItemWidth(100.f);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100.f);
			if (ImGui::Button("Save"))
			{
				shader->SaveShader(m_nameShader);
			}
			ImGui::HelpMarkerItem("Save the material");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
			ImGui::SetNextItemWidth(100.f);
			if (ImGui::Button("Discard"))
			{
				shader->LoadShader(m_nameShader);
			}
			ImGui::HelpMarkerItem("Discard all changes");
			ImGui::PopStyleColor();
		}
	}

	void WindowShader::DisplayVertAndFragShader(std::shared_ptr<Resources::Shader>& shader, bool isVertex)
	{
		std::string name;
		std::string path;

		if (isVertex)
		{
			path = shader->GetVertName();
			name = "Vertex";
		}
		else
		{
			path = shader->GetFragName();
			name = "Fragment";
		}

		ImGui::RButtonDD(name.c_str(),
			!path.empty() ? (ICON_FA_FILE "  " + std::filesystem::path(path).filename().string()).c_str() : "");
		if (!path.empty())
			ImGui::HelpMarkerItem(path.c_str());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
			{
				const std::string& data = reinterpret_cast<const char*>(payload->Data);
				if (isVertex)
					shader->SetVertName(data);
				else
					shader->SetFragName(data);

				//ImGui::EndDragDropTarget();
			}
			ImGui::EndDragDropTarget();
		}
	}

	void WindowShader::LockSelectedShaderButton()
	{
		if (m_isLocked)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
			if (ImGui::Button(ICON_FA_LOCK))
				m_isLocked = false;
			ImGui::HelpMarkerItem("Locked");
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
			if (ImGui::Button(ICON_FA_UNLOCK))
				m_isLocked = true;
			ImGui::HelpMarkerItem("Unlocked");
			ImGui::PopStyleColor();
		}
	}
	void WindowShader::Tick()
	{
		if (!m_isLocked && m_shader != GetEngine()->shaderSelected)
		{
			m_shader = GetEngine()->shaderSelected;
			m_nameShader = GetEngine()->nameShaderSelected;
		}


		if (m_shader)
		{
			m_nameShader = GetEngine()->GetResourcesManager()->FindShaderFromShared(m_shader);
			LockSelectedShaderButton();
			ImGui::SameLine();
			ImGui::Text(m_nameShader.c_str());
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ShaderInspector(m_shader);
		}
	}
}