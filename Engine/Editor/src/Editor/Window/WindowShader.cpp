#include <glad\glad.h>
#include <filesystem>

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

	void WindowShader::SaveDiscard(std::shared_ptr<Resources::Shader>& shader)
	{
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 120.f);
		if (ImGui::Button("Save", { 60.f, 0.f }))
			shader->SaveShader(m_nameShader);

		ImGui::SameLine();

		if (ImGui::Button("Discard", { 60.f, 0.f }))
			shader->LoadShader(m_nameShader);
	}

	void WindowShader::ShaderInspector(std::shared_ptr<Resources::Shader>& shader)
	{
		m_nameShader = GetEngine()->GetResourcesManager()->FindPathFromShared(m_shader);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputText("## DisplayShaderName", (char*)m_nameShader.c_str(), m_nameShader.size(), ImGuiInputTextFlags_ReadOnly);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::CollapsingHeaderWithImageUV(m_propertyIcon->texture, "Properties", m_treeNodeFlags))
		{
			ImGui::Spacing();

			DisplayVertAndFragShader(shader, true);
			DisplayVertAndFragShader(shader, false);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		SaveDiscard(shader);

		ImGui::Spacing();
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
			}
			ImGui::EndDragDropTarget();
		}
	}

	void WindowShader::LockSelectedShaderButton()
	{
		if (m_isLocked)
		{
			if (ImGui::Button(ICON_FA_LOCK))
				m_isLocked = false;
			ImGui::HelpMarkerItem("Locked");
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
			if (m_shader.use_count() <= 2)
			{
				m_shader = nullptr;
				if (GetEngine()->shaderSelected == m_shader)
					GetEngine()->shaderSelected = nullptr;
				return;
			}
			LockSelectedShaderButton();
			ImGui::SameLine();
			ShaderInspector(m_shader);
		}
	}
}
