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
		AWindow{ canvas, ICON_FA_INFO_CIRCLE "  Inspector", visible }
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
			SetObjectStatic(false, object);
		}

		if (isOpen)
		{
			ImGui::Spacing();
			DisplayObjectFlags(object);

			if (GetEngine()->gizmoMode == ImGuizmo::MODE::LOCAL)
				DisplayObjectLocalTransform(object);
			else
				DisplayObjectGlobalTransform(object);

			ImGui::Spacing();
		}
	}

	void WindowInspector::SetObjectStatic(bool isStatic, Core::Datastructure::Object* object)
	{
		if (isStatic != object->GetFlags().test_flag(Core::Datastructure::ObjectFlags::STATIC))
			rttr::type::get<Core::Datastructure::Object>().get_property("flags").set_value(object, object->GetFlags() ^ Core::Datastructure::ObjectFlags::STATIC);

		if (isStatic)
		{
			for (Core::Datastructure::Object* it : object->GetChildren())
				SetObjectStatic(true, it);
		}
	}

	void WindowInspector::DisplayObjectFlags(Core::Datastructure::Object* object)
	{
		bool b{ object->GetFlags().test_flag(Core::Datastructure::ObjectFlags::STATIC) };
		ImGui::RCheckbox("Is static", &b);
		SetObjectStatic(b, object);
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

	// Function to resize strings
	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = static_cast<std::string*>(data->UserData);
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		return 0;
	}

	void WindowInspector::DrawProperty(rttr::property prop, rttr::instance component)
	{
		const rttr::type propType{ prop.get_type() };
		rttr::variant needDisplay{ prop.get_metadata(MetaData_Type::SHOW_IN_EDITOR) };
		if (needDisplay.is_valid())
		{
			if (needDisplay.get_type() == rttr::type::get<bool>())
			{
				if (!needDisplay.get_value<bool>())
					return;
			}
			else if (needDisplay.get_type() == rttr::type::get<std::string>())
			{
				rttr::method m{ component.get_derived_type().get_method(needDisplay.get_value<std::string>()) };
				if (m.is_valid())
				{
					rttr::variant v{ m.invoke(component) };
					if (v.is_valid() && v.is_type<bool>() && !v.get_value<bool>())
						return;
				}
				else
					BAKERS_LOG_WARNING("Could not find method of name " + needDisplay.get_value<std::string>() + " in component " + component.get_derived_type().get_name());
			}
			else
				BAKERS_LOG_WARNING("Property " + prop.get_name().to_string() + "'s metadata SHOW_IN_EDITOR does not have authorized type");
		}

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
		else if (propType == rttr::type::get<std::vector<std::string>>())
		{
			std::vector<std::string> vectorStr = prop.get_value(component).get_value<std::vector<std::string>>();
			bool needToUpdate{ false };
			for (size_t i{ 0 }; i < vectorStr.size(); i++)
			{
				if (DisplayStringDD(prop, vectorStr[i]))
					needToUpdate = true;
			}
			if(needToUpdate)
				prop.set_value(component, vectorStr);
		}
		else if (propType == rttr::type::get<std::string>())
		{
			std::string str{ prop.get_value(component).get_value<std::string>() };
			
			rttr::variant needDisplay{ prop.get_metadata(MetaData_Type::STRING_EDITABLE) };
			bool	inputText = false;
			if (needDisplay.is_valid())
			{
				if (needDisplay.get_type() == rttr::type::get<bool>())
				{
					if (needDisplay.get_value<bool>())
						inputText = true;
				}
				else if (needDisplay.get_type() == rttr::type::get<std::string>())
				{
					rttr::method m{ component.get_derived_type().get_method(needDisplay.get_value<std::string>()) };
					if (m.is_valid())
					{
						rttr::variant v{ m.invoke(component) };
						if (v.is_valid() && v.is_type<bool>() && v.get_value<bool>())
							inputText = true;
					}
					else
						BAKERS_LOG_WARNING("Could not find method of name " + needDisplay.get_value<std::string>() + " in component " + component.get_derived_type().get_name());
				}
				else
					BAKERS_LOG_WARNING("Property " + prop.get_name().to_string() + "'s metadata SHOW_IN_EDITOR does not have authorized type");
			}
			if (inputText)
			{
				ImGui::PushItemToRight(prop.get_name().to_string().c_str());
				if (ImGui::InputText("", const_cast<char*>(str.c_str()), str.capacity() + 1, ImGuiInputTextFlags_CallbackResize, InputTextCallback, &str))
					prop.set_value(component, str);
			}
			else if (DisplayStringDD(prop, str))
				prop.set_value(component, str);
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

	bool WindowInspector::DisplayStringDD(rttr::property prop, std::string& str)
	{
		ImGui::RButtonDD(prop.get_name().to_string().c_str(),
			!str.empty() ? (ICON_FA_FILE "  " + std::filesystem::path(str).filename().string()).c_str() : "");
		if (!str.empty())
			ImGui::HelpMarkerItem(str.c_str());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
			{
				const std::string& data = reinterpret_cast<const char*>(payload->Data);
				str = data;
				ImGui::EndDragDropTarget();
				return true;
			}
			ImGui::EndDragDropTarget();
		}

		return false;
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

	bool CompareType(const rttr::type& first, const rttr::type& second)
	{
		return first.get_name() < second.get_name();
	}

	void WindowInspector::AddComponentToObjectButton(Core::Datastructure::Object* object)
	{
		ImGui::PushStyleColor(ImGuiCol_Button,			{ 0.88f, 0.70f, 0.17f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	{ 0.88f, 0.70f, 0.17f, 0.70f });

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() * ((1.f - 0.7f) * 0.5f));
		if (ImGui::BeginComboButton("Add component", { ImGui::GetWindowContentRegionWidth() * 0.7f, 0 }))
		{
			rttr::array_range possibleComponents{ rttr::type::get<Core::Datastructure::ComponentBase>().get_derived_classes() };
			std::list<rttr::type>	sortedComponents;
			for (auto it : possibleComponents)
			{
				sortedComponents.push_back(it);
			}
			sortedComponents.sort(CompareType);
			for (auto it : sortedComponents)
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

	void WindowInspector::Tick()
	{
		if (!m_isLocked && m_object != GetEngine()->objectSelected)
			m_object = GetEngine()->objectSelected;

		if (m_object)
		{
			LockSelectedObjectButton();
			ImGui::SameLine();
			ObjectInspector(m_object);
		}
	}
}
