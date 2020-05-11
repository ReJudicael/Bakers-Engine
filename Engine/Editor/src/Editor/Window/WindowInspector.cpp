#include <iostream>
#include <filesystem>

#include "WindowInspector.h"
#include "RootObject.hpp"
#include "Maths.hpp"
#include "ScriptedComponent.h"
#include "Reflection.h"
#include "EditorEngine.h"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowInspector::WindowInspector(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Inspector", visible }
	{
		m_treeNodeFlags		= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		m_inputTextFlags	= ImGuiInputTextFlags_AutoSelectAll;
	}

	void WindowInspector::PushWindowStyle()
	{
	}

	void WindowInspector::PopWindowStyle()
	{
	}

	void WindowInspector::DisplayObjectName(Core::Datastructure::Object* object)
	{
		const std::string& getObjectSelected = object->GetName();
		if (strncmp(m_name, getObjectSelected.c_str(), getObjectSelected.size() + 1) != 0)
			memcpy(m_name, getObjectSelected.c_str(), getObjectSelected.size() + 1);

		ImGui::SetNextItemWidth(-FLT_MIN);

		if (ImGui::InputText("## DisplayObjectName", m_name, IM_ARRAYSIZE(m_name), m_inputTextFlags))
			object->SetName(m_name);
	}

	void WindowInspector::DisplayObjectLocalTransform(Core::Datastructure::Object* object)
	{
		Core::Maths::Vec3 pos{ object->GetPos() };
		if (ImGui::RDragFloat3("Position", pos.xyz, 0.01f))
			object->SetPos(pos);

		Core::Maths::Vec3 rot{ object->GetRot().ToEulerAngles() };
		rot *= static_cast<float>(RAD2DEG);
		if (ImGui::RDragFloat3("Rotation", rot.xyz, 0.01f))
			object->SetRot(rot * static_cast<float>(DEG2RAD));

		Core::Maths::Vec3 scale{ object->GetScale() };
		if (ImGui::RDragFloat3("Scale", scale.xyz, 0.01f))
			object->SetScale(scale);
	}

	void WindowInspector::DisplayObjectGlobalTransform(Core::Datastructure::Object* object)
	{
		Core::Maths::Vec3 pos{ object->GetGlobalPos() };
		if (ImGui::RDragFloat3("Position", pos.xyz, 0.01f))
			object->SetGlobalPos(pos);

		Core::Maths::Vec3 rot{ object->GetGlobalRot().ToEulerAngles() };
		rot *= static_cast<float>(RAD2DEG);
		if (ImGui::RDragFloat3("Rotation", rot.xyz, 0.01f))
			object->SetGlobalRot(rot * static_cast<float>(DEG2RAD));

		Core::Maths::Vec3 scale{ object->GetGlobalScale() };
		if (ImGui::RDragFloat3("Scale", scale.xyz, 0.01f))
			object->SetGlobalScale(scale);
	}

	GLuint WindowInspector::GetIcon(const std::string& componentName)
	{
		auto it{ m_icons.find(componentName) };
		if (it == m_icons.end())
		{
			const std::string iconsPath{ "Resources\\Images\\InspectorIcons\\" + componentName + ".png" };
			std::shared_ptr<Resources::Texture> icon;
			GetEngine()->GetResourcesManager()->LoadTexture(iconsPath, icon);
			it = m_icons.emplace(componentName, icon).first;
		}

		return it->second->texture;
	}

	void WindowInspector::DisplayObjectTransform(Core::Datastructure::Object* object)
	{
		bool isOpen = ImGui::CollapsingHeaderWithImageUV(GetIcon("Transform"), "Transform", m_treeNodeFlags);
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 16);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
		bool isClickedReset = ImGui::Button(ICON_FA_UNDO);
		ImGui::PopStyleColor();
		ImGui::HelpMarkerItem("Reset");

		if (isClickedReset)
		{
			object->SetPos({ 0.f, 0.f, 0.f });
			object->SetRot({ 0.f, 0.f, 0.f });
			object->SetScale({ 1.f, 1.f, 1.f });
		}

		if (isOpen)
		{
			ImGui::Spacing();

			if (GetEngine()->gizmoMode == ImGuizmo::MODE::LOCAL)
				DisplayObjectLocalTransform(object);
			else
				DisplayObjectGlobalTransform(object);

			ImGui::Spacing();
		}
	}

	void WindowInspector::DrawEnum(rttr::property prop, rttr::instance component)
	{
		rttr::enumeration e{ prop.get_enumeration() };
		if (ImGui::RBeginCombo(prop.get_name().to_string().c_str(), e.value_to_name(prop.get_value(component)).to_string().c_str()))
		{
			for (const auto& name : e.get_names())
			{
				if (ImGui::Selectable(name.to_string().c_str()))
					prop.set_value(component, e.name_to_value(name));
			}
			ImGui::EndCombo();
		}
	}

	void WindowInspector::DrawProperty(rttr::property prop, rttr::instance component)
	{
		const rttr::type propType{ prop.get_type() };

		if (!propType.is_valid())
		{
			ImGui::Text("Invalid type");
			return;
		}

		if (prop.is_enumeration())
		{
			DrawEnum(prop, component);
		}
		else if (propType == rttr::type::get<bool>())
		{
			bool b{ prop.get_value(component).get_value<bool>() };
			if (ImGui::RCheckbox(prop.get_name().to_string().c_str(), &b) && !prop.is_readonly())
				prop.set_value(component, b);
		}
		else if (propType == rttr::type::get<int>())
		{
			int i{ prop.get_value(component).get_value<int>() };
			if (ImGui::RDragInt(prop.get_name().to_string().c_str(), &i, 1.0f * !prop.is_readonly()) && !prop.is_readonly())
				prop.set_value(component, i);
		}
		else if (propType == rttr::type::get<float>())
		{
			float f{ prop.get_value(component).get_value<float>() };
			if (ImGui::RDragFloat(prop.get_name().to_string().c_str(), &f, 0.01f * !prop.is_readonly()) && !prop.is_readonly())
				prop.set_value(component, f);
		}
		else if (propType == rttr::type::get<Core::Maths::Vec2>())
		{
			Core::Maths::Vec2 v{ prop.get_value(component).get_value<Core::Maths::Vec2>() };
			if (ImGui::RDragFloat2(prop.get_name().to_string().c_str(), v.xy, 0.01f * !prop.is_readonly()) && !prop.is_readonly())
				prop.set_value(component, v);
		}
		else if (propType == rttr::type::get<Core::Maths::Vec3>())
		{
			Core::Maths::Vec3 v{ prop.get_value(component).get_value<Core::Maths::Vec3>() };
			if (ImGui::RDragFloat3(prop.get_name().to_string().c_str(), v.xyz, 0.01f * !prop.is_readonly()) && !prop.is_readonly())
				prop.set_value(component, v);
		}
		else if (propType == rttr::type::get<Core::Maths::Vec4>())
		{
			Core::Maths::Vec4 v{ prop.get_value(component).get_value<Core::Maths::Vec4>() };
			if (ImGui::RDragFloat4(prop.get_name().to_string().c_str(), v.xyzw, 0.01f * !prop.is_readonly()) && !prop.is_readonly())
				prop.set_value(component, v);
		}
		else if (propType == rttr::type::get<Core::Maths::Color>())
		{
			Core::Maths::Color c{ prop.get_value(component).get_value<Core::Maths::Color>() };
			if (ImGui::RColorEdit4(prop.get_name().to_string().c_str(), c.rgba) && !prop.is_readonly())
				prop.set_value(component, c);
		}
		else if (propType == rttr::type::get<std::string>())
		{
			const std::string str{ prop.get_value(component).get_value<std::string>() };
			ImGui::RButtonDD(prop.get_name().to_string().c_str(),
				!str.empty() ? (ICON_FA_FILE "  " + std::filesystem::path(str).filename().string()).c_str() : "");
			if (!str.empty())
				ImGui::HelpMarkerItem(str.c_str());

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
				{
					const std::string& path{ reinterpret_cast<const char*>(payload->Data) };
					prop.set_value(component, path);
					ImGui::EndDragDropTarget();
				}
			}
		}
		else if (propType.is_class())
		{
			const rttr::variant& temp{ prop.get_value(component) };
			DisplayInstance(prop.get_type(), temp);
			if (!prop.is_readonly())
				prop.set_value(component, temp);
		}
		else
		{
			ImGui::Text("Type: %s\nName: %s\nValue: %s\n\n",
				propType.get_name().to_string().c_str(),
				prop.get_name().to_string().c_str(),
				prop.get_value(component).to_string().c_str());
		}
	}

	void WindowInspector::DisplayObjectComponents(Core::Datastructure::Object* object)
	{
		for (auto it : object->GetComponents())
		{
			ImGui::PushID(it);

			rttr::type t{ it->get_type() };
			bool isOpen = ImGui::CollapsingHeaderWithImageUV(GetIcon(t.get_name().to_string().c_str()), t.get_name().to_string().c_str(), m_treeNodeFlags);

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });

			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 40.f);

			bool isClickedReset = ImGui::Button(ICON_FA_UNDO);
			ImGui::HelpMarkerItem("Reset");

			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 16.f);
			bool isClickedDelete = ImGui::Button(ICON_FA_TRASH);
			ImGui::HelpMarkerItem("Remove");

			ImGui::PopStyleColor();

			if (isClickedDelete)
			{
				object->EraseComponent(it);
				ImGui::PopID();
				return;
			}

			if (isClickedReset)
				it->Reset();

			if (isOpen)
				DisplayInstance(t, *it);

			ImGui::PopID();
		}
	}

	void WindowInspector::DisplayInstance(rttr::type t, rttr::instance inst)
	{
		ImGui::Spacing();
		for (const auto& prop : t.get_properties())
		{
			if (prop.get_access_level() == rttr::access_levels::public_access)
				DrawProperty(prop, inst);
		}
		ImGui::Spacing();
	}

	void WindowInspector::AddComponentToObjectButton(Core::Datastructure::Object* object)
	{
		ImGui::PushStyleColor(ImGuiCol_Button,			{ 0.88f, 0.70f, 0.17f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	{ 0.88f, 0.70f, 0.17f, 0.70f });

		ImGui::Indent(ImGui::GetWindowContentRegionWidth() * ((1 - 0.7f) / 2));
		if (ImGui::BeginComboButton("Add component", { ImGui::GetWindowContentRegionWidth() * 0.7f, 0 }))
		{
			rttr::array_range possibleComponents{ rttr::type::get<Core::Datastructure::ComponentBase>().get_derived_classes() };
			for (auto it : possibleComponents)
			{
				if (ImGui::MenuItem(it.get_name().to_string().c_str()))
					object->AddComponent(it.invoke("GetCopy", it.create(), {}).get_value<Core::Datastructure::ComponentBase*>());
			}
			ImGui::EndCombo();
		}

		ImGui::PopStyleColor(2);
	}

	void WindowInspector::ObjectInspector(Core::Datastructure::Object* object)
	{
		if (object->IsDestroyed())
			return;
		DisplayObjectName(object);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		DisplayObjectTransform(object);
		DisplayObjectComponents(object);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		AddComponentToObjectButton(object);
		ImGui::Spacing();
	}

	void WindowInspector::LockSelectedObjectButton()
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

	void WindowInspector::Tick()
	{
		/*if (!m_isLocked && m_inspectorObject != GetEngine()->objectSelected)
			m_inspectorObject = GetEngine()->objectSelected;

		if (m_inspectorObject)
		{
			LockSelectedObjectButton();
			ImGui::SameLine();
			ObjectInspector(m_inspectorObject);
		}*/

		std::shared_ptr<Resources::Material> mat = GetEngine()->materialSelected;
		if (mat)
		{
			ImGui::SliderFloat("shininess ", &mat->shininess, 0.f, 5.f);
			ImGui::SliderFloat3("ambient ", mat->ambientColor.gba, 0.f, 5.f);

			for (auto i{ 0 }; i < 1; i++)
			{

				std::string str = mat->textures[i]->name;

					ImGui::RButtonDD("Texture ",
						!str.empty() ? (ICON_FA_FILE "  " + std::filesystem::path(str).filename().string()).c_str() : "");
				if (!str.empty())
					ImGui::HelpMarkerItem(str.c_str());

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
					{
						const std::string& path{ reinterpret_cast<const char*>(payload->Data) };

						GetEngine()->GetResourcesManager()->LoadTexture(path, mat->textures[i]);

						// Use "path" in your function
						ImGui::EndDragDropTarget();
					}
				}
			}
		}
	}
}
