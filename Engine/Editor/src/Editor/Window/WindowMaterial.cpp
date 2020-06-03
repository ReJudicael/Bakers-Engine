#include <glad\glad.h>
#include <filesystem>

#include "WindowMaterial.h"
#include "EditorEngine.h"
#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowMaterial::WindowMaterial(Canvas* canvas, bool visible) :
		AWindow{ canvas, ICON_FA_FILL "  Material", visible }
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

		std::string nameShader = GetEngine()->GetResourcesManager()->FindPathFromShared(m_material->shader);

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

	void WindowMaterial::ShowMaterialVariant(Resources::VariantUniform& varUniform)
	{
		if (varUniform.var.get_type() == rttr::type::get<bool>())
		{
			ImGui::Checkbox(varUniform.name.c_str(), &varUniform.var.get_value<bool>());
		}
		else if (varUniform.var.get_type() == rttr::type::get<int>())
		{
			ImGui::DragInt(varUniform.name.c_str(), &varUniform.var.get_value<int>());
		}
		else if (varUniform.var.get_type() == rttr::type::get<float>())
		{
			ImGui::RDragFloat(varUniform.name.c_str(), &varUniform.var.get_value<float>(), 0.01f);
		}
		else if (varUniform.var.get_type() == rttr::type::get<Core::Maths::Vec2>())
		{
			ImGui::RDragFloat2(varUniform.name.c_str(), varUniform.var.get_value<Core::Maths::Vec2>().xy);
		}
		else if (varUniform.var.get_type() == rttr::type::get<Core::Maths::Vec3>())
		{
			ImGui::RDragFloat3(varUniform.name.c_str(), varUniform.var.get_value<Core::Maths::Vec3>().xyz);
		}
		else if (varUniform.var.get_type() == rttr::type::get<Core::Maths::Vec4>())
		{
			ImGui::RDragFloat4(varUniform.name.c_str(), varUniform.var.get_value<Core::Maths::Vec4>().xyzw);
		}
		else if (varUniform.var.get_type() == rttr::type::get<Core::Maths::Color>())
		{
			ImGui::RColorEdit4(varUniform.name.c_str(), varUniform.var.get_value<Core::Maths::Color>().rgba);
		}
		else if (varUniform.var.get_type() == rttr::type::get<std::shared_ptr<Resources::Texture>>())
		{
			ShowMaterialTexture(varUniform.var.get_value<std::shared_ptr<Resources::Texture>>());
		}
	}

	void WindowMaterial::ShowMaterialTexture(std::shared_ptr<Resources::Texture>& texture)
	{
		std::string nameTexture;
		if (texture)
			nameTexture = GetEngine()->GetResourcesManager()->FindPathFromShared(texture);

		DragDropTargetItem(nameTexture, texture);
	}

	void WindowMaterial::MaterialInspector(std::shared_ptr<Resources::Material>& material)
	{
		m_nameMaterial = GetEngine()->GetResourcesManager()->FindPathFromShared(m_material);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputText("## DisplayMaterialName", (char*)m_nameMaterial.c_str(), m_nameMaterial.size(), ImGuiInputTextFlags_ReadOnly);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::CollapsingHeaderWithImageUV(m_propertyIcon->texture, "Properties", m_treeNodeFlags))
		{
			ImGui::Spacing();
			ShowMaterialShaders();
			ImGui::RColorEdit4("Ambient", material->ambientColor.rgba);
			ImGui::RColorEdit4("Diffuse", material->diffuseColor.rgba);
			ImGui::RColorEdit4("Specular", material->specularColor.rgba);
			ImGui::RDragFloat("Shininess", &material->shininess, 0.01f);
			ImGui::RDragFloat("Shininess Strength", &material->shininessStrength, 0.01f);

			if (material->textures.size() > 0)
			{
				ImGui::Spacing();
				if (ImGui::TreeNodeEx("Base Texture", ImGuiTreeNodeFlags_DefaultOpen))
				{
					for (auto& texture : material->textures)
						ShowMaterialTexture(texture);
					ImGui::TreePop();
				}
			}

			if (material->variants.size() > 0)
			{
				ImGui::Spacing();
				if (ImGui::TreeNodeEx("Uniform Personnal", ImGuiTreeNodeFlags_DefaultOpen))
				{
					for (auto& varUniform : material->variants)
						ShowMaterialVariant(varUniform);
					ImGui::TreePop();
				}
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		SaveDiscard(material);
		ImGui::Spacing();
	}

	void WindowMaterial::SaveDiscard(std::shared_ptr<Resources::Material>& material)
	{
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 120.f);

		if (ImGui::Button("Save", { 60.f, 0.f }))
			material->SaveMaterial(m_nameMaterial, GetEngine()->GetResourcesManager());

		ImGui::SameLine();

		if (ImGui::Button("Discard", { 60.f, 0.f }))
			material->LoadMaterialFromJSON(m_nameMaterial, GetEngine()->GetResourcesManager());
	}

	void WindowMaterial::LockSelectedMaterialButton()
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

	void WindowMaterial::Tick()
	{
		if (!m_isLocked && m_material != GetEngine()->materialSelected)
		{
			m_material = GetEngine()->materialSelected;
			m_nameMaterial = GetEngine()->nameMaterialSelected;
		}

		if (m_material)
		{
			if (m_material->IsDelete)
			{
				m_material = nullptr;
				return;
			}

			LockSelectedMaterialButton();
			ImGui::SameLine();
			MaterialInspector(m_material);
		}
	}
}
