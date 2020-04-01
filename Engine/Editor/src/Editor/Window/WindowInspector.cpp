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
		m_treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
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

		if (ImGui::InputText("## Name", m_name, IM_ARRAYSIZE(m_name), ImGuiInputTextFlags_AutoSelectAll))
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
		rot *= RAD2DEG;
		if (ImGui::RDragFloat3("Rotation", rot.xyz))
			object->SetRot(rot * DEG2RAD);

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
		rot *= RAD2DEG;
		if (ImGui::RDragFloat3("Rotation", rot.xyz))
			object->SetGlobalRot(rot * DEG2RAD);

		Core::Maths::Vec3 scale{ object->GetGlobalScale() };
		if (ImGui::RDragFloat3("Scale", scale.xyz, 0.01f))
			object->SetGlobalScale(scale);
	}

	void WindowInspector::DisplayObjectTransform(Core::Datastructure::Object* object)
	{
		if (ImGui::CollapsingHeader("Transform", m_treeNodeFlags))
		{
			ImGui::Spacing();

			if (m_isLocal)
				DisplayObjectLocalTransform(object);
			else
				DisplayObjectGlobalTransform(object);

			ImGui::Spacing();
		}
	}

	void WindowInspector::Tick()
	{
		if (GetEngine()->objectSelected == nullptr)
			return;

		DisplayObjectName(GetEngine()->objectSelected);
		ImGui::Spacing();
		DisplayObjectTransform(GetEngine()->objectSelected);

		for (auto it : GetEngine()->objectSelected->GetComponents())
		{
			type t = it->get_type();
			ImGui::PushID(it);
			if (ImGui::CollapsingHeader(t.get_name().to_string().c_str(), m_treeNodeFlags))
			{
				for (const auto& prop : t.get_properties())
				{
					if (prop.is_readonly())
						continue;
					DrawProperty(prop, it);
				}
			}
			ImGui::PopID();
		}

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::SetNextItemCenter(0.7f);
		if (ImGui::BeginComboColoredButton("Add component", { 0.88f, 0.70f, 0.17f }))
		{
			array_range possibleComponents = type::get<Core::Datastructure::ComponentBase>().get_derived_classes();
			for (auto it : possibleComponents)
			{
				if (ImGui::Selectable(it.get_name().to_string().c_str()))
					GetEngine()->objectSelected->AddComponent(it.invoke("GetCopy", it.create(), {}).get_value<Core::Datastructure::ComponentBase*>());
			}
			ImGui::EndCombo();
		}
		ImGui::Dummy({ 0.f, 90.f });
	}

	void WindowInspector::DrawEnum(rttr::property prop, Core::Datastructure::ComponentBase* component)
	{
		rttr::enumeration e{ prop.get_enumeration() };
		ImGui::Text((prop.get_name().to_string() + " : " + e.value_to_name(prop.get_value(component)).to_string()).c_str());
		/* Check here if pressed on the enum to change it
		if ()
		for (auto it : e.get_names())
		{
			
		}
		*/
	}

	void WindowInspector::DrawProperty(rttr::property prop, Core::Datastructure::ComponentBase* component)
	{
		if (!prop.get_type().is_valid())
		{
			ImGui::Text("Invalid type");
			return;
		}
		else if (prop.is_enumeration())
			DrawEnum(prop, component);
		else if (prop.get_type() == type::get<bool>())
		{
			bool b{ prop.get_value(component).get_value<bool>() };
			if (ImGui::Checkbox(prop.get_name().to_string().c_str(), &b))
				prop.set_value(component, b);
		}
		else if (prop.get_type() == type::get<float>())
		{
			float f{ prop.get_value(component).get_value<float>() };
			if (ImGui::DragFloat(prop.get_name().to_string().c_str(), &f))
				prop.set_value(component, f);
		}
		else if (prop.get_type() == type::get<int>())
		{
			int i{ prop.get_value(component).get_value<int>() };
			if (ImGui::DragInt(prop.get_name().to_string().c_str(), &i))
				prop.set_value(component, i);
		}
		else
		{
			ImGui::Text("type: %s\nname: %s\nvalue: %s\n\n",
				prop.get_type().get_name().to_string().c_str(),
				prop.get_name().to_string().c_str(),
				prop.get_value(*component).to_string().c_str());
		}
	}
}
