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
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.58f, 0.20f, 1.00f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.58f, 0.20f, 0.70f });
		{
			if (ImGui::Button("Local", { ImGui::GetContentRegionAvail().x, 0.f }))
				m_isLocal = !m_isLocal;
		}
		ImGui::PopStyleColor(2);

		Core::Maths::Vec3 pos{ object->GetPos() };
		if (ImGui::DragFloat3("Position", pos.xyz))
			object->SetPos(pos);

		Core::Maths::Vec3 rot{ object->GetRot().ToEulerAngles() };
		rot *= RAD2DEG;
		if (ImGui::DragFloat3("Rotation", rot.xyz))
			object->SetRot(rot * DEG2RAD);

		Core::Maths::Vec3 scale{ object->GetScale() };
		if (ImGui::DragFloat3("Scale", scale.xyz, 0.01f))
			object->SetScale(scale);
	}

	void WindowInspector::DisplayObjectGlobalTransform(Core::Datastructure::Object* object)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.60f, 0.35f, 0.71f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.60f, 0.35f, 0.71f, 0.70f });
		{
			if (ImGui::Button("Global", { ImGui::GetContentRegionAvail().x, 0.f }))
				m_isLocal = !m_isLocal;
		}
		ImGui::PopStyleColor(2);

		Core::Maths::Vec3 pos{ object->GetGlobalPos() };
		if (ImGui::DragFloat3("Position", pos.xyz))
			object->SetGlobalPos(pos);

		Core::Maths::Vec3 rot{ object->GetGlobalRot().ToEulerAngles() };
		rot *= RAD2DEG;
		if (ImGui::DragFloat3("Rotation", rot.xyz))
			object->SetGlobalRot(rot * DEG2RAD);

		Core::Maths::Vec3 scale{ object->GetGlobalScale() };
		if (ImGui::DragFloat3("Scale", scale.xyz, 0.01f))
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
		}
	}

	void WindowInspector::Tick()
	{
		if (GetEngine()->objectSelected == nullptr)
			return;

		DisplayObjectName(GetEngine()->objectSelected);
		ImGui::Separator();
		DisplayObjectTransform(GetEngine()->objectSelected);
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Properties", m_treeNodeFlags))
		{
			for (auto it = GetEngine()->objectSelected->GetComponents().begin(); it != GetEngine()->objectSelected->GetComponents().end(); ++it)
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
