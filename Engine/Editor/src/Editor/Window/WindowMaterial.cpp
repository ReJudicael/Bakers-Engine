#include <glad\glad.h>

#include "WindowMaterial.h"
#include "EditorEngine.h"
#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowMaterial::WindowMaterial(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Material", visible }
	{
		m_treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\InspectorIcons\\Material.png", m_propertyIcon);
	}

	void WindowMaterial::PushWindowStyle()
	{
	}

	void WindowMaterial::PopWindowStyle()
	{
	}

	void WindowMaterial::ShowMaterialShaders()
	{
		const auto& shader = GetEngine()->GetResourcesManager()->GetShaderMap();

		std::string nameShader;
		for (auto it = shader.begin(); it != shader.end(); ++it)
		{
			if (it->second == m_material->shader)
			{
				nameShader = it->first;
				break;
			}
		}

		if (ImGui::RBeginCombo("Type Shader", nameShader.c_str()))
		{
			for (auto it = shader.begin(); it != shader.end(); ++it)
			{
				if (ImGui::Selectable(it->first.c_str()))
					m_material->UpdateMaterialShader(it->second);
			}
			ImGui::EndCombo();
		}
	}

	void WindowMaterial::DragDropTargetItem(const std::string& itemPath, std::shared_ptr<Resources::Texture>& texture)
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

	void WindowMaterial::ShowMaterialVariant(Resources::VariantUniform varUniform)
	{
		if (varUniform.var.can_convert<float>())
		{
			ImGui::RDragFloat(varUniform.name.c_str(), &varUniform.var.get_value<float>(), 0.01f);
		}
		else if (varUniform.var.can_convert<std::shared_ptr<Resources::Texture>>())
		{
			std::shared_ptr<Resources::Texture> text = varUniform.var.get_value<std::shared_ptr<Resources::Texture>>();
			std::string str;
			if (text)
				str = text->name;

			DragDropTargetItem(str, varUniform.var.get_value<std::shared_ptr<Resources::Texture>>());
		}
	}

	void WindowMaterial::ShowMaterialTexture(std::shared_ptr<Resources::Texture>& texture)
	{
		std::string str;
		if (texture)
			str = texture->name;

		DragDropTargetItem(str, texture);
	}

	void WindowMaterial::MaterialInspector(std::shared_ptr<Resources::Material>& material)
	{
		if (ImGui::CollapsingHeaderWithImageUV(m_propertyIcon->texture, "Properties", m_treeNodeFlags))
		{
			ImGui::Spacing();
			ShowMaterialShaders();
			ImGui::RDragFloat("Shininess", &GetEngine()->materialSelected->shininess, 0.01f);
			ImGui::RColorEdit4("Ambient", GetEngine()->materialSelected->ambientColor.rgba);
			ImGui::RColorEdit4("Diffuse", GetEngine()->materialSelected->diffuseColor.rgba);
			ImGui::RColorEdit4("Specular", GetEngine()->materialSelected->specularColor.rgba);
			for (auto& varUniform : GetEngine()->materialSelected->variants)
				ShowMaterialVariant(varUniform);
			for (auto& texture : GetEngine()->materialSelected->textures)
				ShowMaterialTexture(texture);
		}
	}

	void WindowMaterial::LockSelectedMaterialButton()
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
	void WindowMaterial::Tick()
	{
		if (!m_isLocked && m_material != GetEngine()->materialSelected)
			m_material = GetEngine()->materialSelected;

		if (m_material)
		{
			LockSelectedMaterialButton();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			MaterialInspector(GetEngine()->materialSelected);
		}
	}
}
