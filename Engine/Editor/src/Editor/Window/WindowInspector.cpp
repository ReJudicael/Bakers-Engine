#include "WindowInspector.h"
#include "Reflection.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include "Maths.hpp"
#include <iostream>

namespace Editor::Window
{
	WindowInspector::WindowInspector(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Inspector", visible }
	{
		m_treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		m_inputTextFlags = ImGuiInputTextFlags_AutoSelectAll;

		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\InspectorIcons\\delete.png", m_deleteIcon);
		GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\InspectorIcons\\reset.png", m_resetIcon);
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

		if (ImGui::InputText("## Name", m_name, IM_ARRAYSIZE(m_name), m_inputTextFlags))
			object->SetName(m_name);
	}

	void WindowInspector::DisplayObjectLocalTransform(Core::Datastructure::Object* object)
	{
		if (ImGui::ColoredButton("Local", { ImGui::GetContentRegionAvail().x, 0.f }, { 0.00f, 0.58f, 0.20f }))
			m_isLocal = !m_isLocal;

		Core::Maths::Vec3 pos{ object->GetPos() };
		if (ImGui::RDragFloat3("Position", pos.xyz))
			object->SetPos(pos);

		Core::Maths::Vec3 rot{ object->GetRot().ToEulerAngles() };
		rot *= static_cast<float>(RAD2DEG);
		if (ImGui::RDragFloat3("Rotation", rot.xyz))
			object->SetRot(rot * static_cast<float>(DEG2RAD));

		Core::Maths::Vec3 scale{ object->GetScale() };
		if (ImGui::RDragFloat3("Scale", scale.xyz, 0.01f))
			object->SetScale(scale);
	}

	void WindowInspector::DisplayObjectGlobalTransform(Core::Datastructure::Object* object)
	{
		if (ImGui::ColoredButton("Global", { ImGui::GetContentRegionAvail().x, 0.f }, { 0.60f, 0.35f, 0.71f }))
			m_isLocal = !m_isLocal;

		Core::Maths::Vec3 pos{ object->GetGlobalPos() };
		if (ImGui::RDragFloat3("Position", pos.xyz))
			object->SetGlobalPos(pos);

		Core::Maths::Vec3 rot{ object->GetGlobalRot().ToEulerAngles() };
		rot *= static_cast<float>(RAD2DEG);
		if (ImGui::RDragFloat3("Rotation", rot.xyz))
			object->SetGlobalRot(rot * static_cast<float>(DEG2RAD));

		Core::Maths::Vec3 scale{ object->GetGlobalScale() };
		if (ImGui::RDragFloat3("Scale", scale.xyz, 0.01f))
			object->SetGlobalScale(scale);
	}

	void WindowInspector::DisplayObjectTransform(Core::Datastructure::Object* object)
	{
		bool isOpen = ImGui::CollapsingHeader("Transform", m_treeNodeFlags);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 16);
#pragma warning(suppress : 4312)
		bool isClickedReset = ImGui::ImageButtonUV_HelpMarker(m_resetIcon->texture, "Reset");
		ImGui::PopStyleColor(1);

		if (isClickedReset)
		{
			// Reset Transform
		}

		if (isOpen)
		{
			ImGui::Spacing();

			if (m_isLocal)
				DisplayObjectLocalTransform(object);
			else
				DisplayObjectGlobalTransform(object);

			ImGui::Spacing();
		}
	}

	void WindowInspector::DrawEnum(rttr::property prop, rttr::instance component)
	{
		rttr::enumeration e{ prop.get_enumeration() };
		if (ImGui::RBeginCombo(prop.get_name().to_string(), e.value_to_name(prop.get_value(component)).to_string().c_str()))
		{
			for (auto name : e.get_names())
			{
				if (ImGui::Selectable(name.to_string().c_str()))
					prop.set_value(component, e.name_to_value(name));
			}
			ImGui::EndCombo();
		}
	}

	void WindowInspector::DrawProperty(rttr::property prop, rttr::instance component)
	{
		if (!prop.get_type().is_valid())
		{
			ImGui::Text("Invalid type");
			return;
		}
		else if (prop.is_enumeration())
		{
			DrawEnum(prop, component);
		}
		else if (prop.get_type() == type::get<bool>())
		{
			bool b{ prop.get_value(component).get_value<bool>() };
			if (ImGui::RCheckbox(prop.get_name().to_string().c_str(), &b))
				prop.set_value(component, b);
		}
		else if (prop.get_type() == type::get<int>())
		{
			int i{ prop.get_value(component).get_value<int>() };
			if (ImGui::RDragInt(prop.get_name().to_string().c_str(), &i))
				prop.set_value(component, i);
		}
		else if (prop.get_type() == type::get<float>())
		{
			float f{ prop.get_value(component).get_value<float>() };
			if (ImGui::RDragFloat(prop.get_name().to_string().c_str(), &f))
				prop.set_value(component, f);
		}
		else if (prop.get_type() == type::get<Core::Maths::Vec2>())
		{
			Core::Maths::Vec2 v{ prop.get_value(component).get_value<Core::Maths::Vec2>() };
			if (ImGui::RDragFloat2(prop.get_name().to_string().c_str(), v.xy, 0.01f))
				prop.set_value(component, v);
		}
		else if (prop.get_type() == type::get<Core::Maths::Vec3>())
		{
			Core::Maths::Vec3 v{ prop.get_value(component).get_value<Core::Maths::Vec3>() };
			if (ImGui::RDragFloat3(prop.get_name().to_string().c_str(), v.xyz, 0.01f))
				prop.set_value(component, v);
		}
		else if (prop.get_type() == type::get<Core::Maths::Vec4>())
		{
			Core::Maths::Vec4 v{ prop.get_value(component).get_value<Core::Maths::Vec4>() };
			if (ImGui::RDragFloat4(prop.get_name().to_string().c_str(), v.xyzw, 0.01f))
				prop.set_value(component, v);
		}
		else if (prop.get_type() == type::get<Core::Maths::Color>())
		{
			Core::Maths::Color c{ prop.get_value(component).get_value<Core::Maths::Color>() };
			if (ImGui::RColorEdit4(prop.get_name().to_string().c_str(), c.rgba))
				prop.set_value(component, c);
		}
		else if (prop.get_type().is_class())
		{
			auto temp{ prop.get_value(component) };
			DisplayInstance(prop.get_type(), temp);
			prop.set_value(component, temp);
		}
		else
		{
			ImGui::Text("Type: %s\nName: %s\nValue: %s\n\n",
				prop.get_type().get_name().to_string().c_str(),
				prop.get_name().to_string().c_str(),
				prop.get_value(component).to_string().c_str());
		}
	}

	void WindowInspector::DisplayObjectComponents(Core::Datastructure::Object* object)
	{
		for (auto it : object->GetComponents())
		{
			ImGui::PushID(it);

			type t = it->get_type();
			bool isOpen = ImGui::CollapsingHeader(t.get_name().to_string().c_str(), m_treeNodeFlags);

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 16);
#pragma warning(suppress : 4312)
			bool isClickedDelete = ImGui::ImageButtonUV_HelpMarker(m_deleteIcon->texture, "Delete");
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 40);
#pragma warning(suppress : 4312)
			bool isClickedReset = ImGui::ImageButtonUV_HelpMarker(m_resetIcon->texture, "Reset");
			ImGui::PopStyleColor(1);

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
			if (!prop.is_readonly())
				DrawProperty(prop, inst);
		}
		ImGui::Spacing();
	}

	void WindowInspector::AddComponentToObjectButton(Core::Datastructure::Object* object)
	{
		if (ImGui::BeginComboColoredButton("Add component", { 0.88f, 0.70f, 0.17f }))
		{
			array_range possibleComponents = type::get<Core::Datastructure::ComponentBase>().get_derived_classes();
			for (auto it : possibleComponents)
			{
				if (ImGui::Selectable(it.get_name().to_string().c_str()))
					object->AddComponent(it.invoke("GetCopy", it.create(), {}).get_value<Core::Datastructure::ComponentBase*>());
			}
			ImGui::EndCombo();
		}
	}

	void WindowInspector::ObjectInspector(Core::Datastructure::Object* object)
	{
		if (object == nullptr)
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

		ImGui::PushItemToCenter(0.7f);
		AddComponentToObjectButton(object);
		ImGui::Spacing();
	}

	void WindowInspector::Tick()
	{
		ObjectInspector(GetEngine()->objectSelected);
	}
}
