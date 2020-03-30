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

	void WindowInspector::DisplayObjectName()
	{
		char name[64];
		memcpy(name, m_objectSelectedInHierarchy->GetName().c_str(), m_objectSelectedInHierarchy->GetName().size() + 1);
		ImGui::SetNextItemWidth(-FLT_MIN);

		if (ImGui::InputText("## Name", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_AutoSelectAll))
			m_objectSelectedInHierarchy->SetName(name);
	}

	void WindowInspector::DisplayObjectLocalTransform()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.58f, 0.20f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.58f, 0.20f, 0.70f });
		{
			if (ImGui::Button("Local", { ImGui::GetContentRegionAvail().x, 0.f }))
				m_isLocal = !m_isLocal;
		}
		ImGui::PopStyleColor(2);

		Core::Maths::Vec3 pos{ m_objectSelectedInHierarchy->GetPos() };
		if (ImGui::DragFloat3("Position", pos.xyz))
			m_objectSelectedInHierarchy->SetPos(pos);

		Core::Maths::Vec3 rot{ m_objectSelectedInHierarchy->GetRot().ToEulerAngles() };
		rot *= RAD2DEG;
		if (ImGui::DragFloat3("Rotation", rot.xyz))
			m_objectSelectedInHierarchy->SetRot(rot * DEG2RAD);

		Core::Maths::Vec3 scale{ m_objectSelectedInHierarchy->GetScale() };
		if (ImGui::DragFloat3("Scale", scale.xyz, 0.01f))
			m_objectSelectedInHierarchy->SetScale(scale);
	}

	void WindowInspector::DisplayObjectGlobalTransform()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.60f, 0.35f, 0.71f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.60f, 0.35f, 0.71f, 0.70f });
		{
			if (ImGui::Button("Global", { ImGui::GetContentRegionAvail().x, 0.f }))
				m_isLocal = !m_isLocal;
		}
		ImGui::PopStyleColor(2);

		Core::Maths::Vec3 pos{ m_objectSelectedInHierarchy->GetGlobalPos() };
		if (ImGui::DragFloat3("Position", pos.xyz))
			m_objectSelectedInHierarchy->SetGlobalPos(pos);

		Core::Maths::Vec3 rot{ m_objectSelectedInHierarchy->GetGlobalRot().ToEulerAngles() };
		rot *= RAD2DEG;
		if (ImGui::DragFloat3("Rotation", rot.xyz))
			m_objectSelectedInHierarchy->SetGlobalRot(rot * DEG2RAD);

		Core::Maths::Vec3 scale{ m_objectSelectedInHierarchy->GetGlobalScale() };
		if (ImGui::DragFloat3("Scale", scale.xyz, 0.01f))
			m_objectSelectedInHierarchy->SetGlobalScale(scale);
	}

	void WindowInspector::DisplayObjectTransform()
	{
		if (ImGui::CollapsingHeader("Transform", m_treeNodeFlags))
		{
			ImGui::Spacing();

			if (m_isLocal)
				DisplayObjectLocalTransform();
			else
				DisplayObjectGlobalTransform();
		}
	}

	void WindowInspector::Tick()
	{
		if (m_objectSelectedInHierarchy != GetEngine()->objectSelected)
			m_objectSelectedInHierarchy = GetEngine()->objectSelected;

		if (m_objectSelectedInHierarchy == nullptr)
			return;

		DisplayObjectName();
		ImGui::Separator();
		DisplayObjectTransform();
		ImGui::Spacing();

		array_range possibleComponents = type::get<Core::Datastructure::ComponentBase>().get_derived_classes();
		for (auto it : possibleComponents)
		{
			if (ImGui::Selectable(it.get_name().to_string().c_str()))
			{
				variant v = it.create();

				GetEngine()->objectSelected->AddComponent(it.invoke("GetCopy", v, {}).get_value<Core::Datastructure::ComponentBase*>());
			}

		}

		if (ImGui::CollapsingHeader("Properties", m_treeNodeFlags))
		{
			for (auto it = m_objectSelectedInHierarchy->GetComponents().begin(); it != m_objectSelectedInHierarchy->GetComponents().end(); ++it)
			{
				type t = (*it)->get_type();
				ImGui::Text(t.get_name().to_string().c_str());
				for (const auto& prop : t.get_properties())
				{
					ImGui::Text("type: %s\nname: %s\nvalue: %s\n\n",
						prop.get_type().get_name().to_string().c_str(),
						prop.get_name().to_string().c_str(),
						prop.get_value(*it).to_string().c_str());
				}
			}
		}
	}
}
