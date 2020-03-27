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
		char name[64];
		memcpy(name, object->GetName().c_str(), object->GetName().size() + 1);
		ImGui::SetNextItemWidth(-FLT_MIN);

		if (ImGui::InputText("## Name", name, IM_ARRAYSIZE(name)))
			object->SetName(name);
	}

	void WindowInspector::DisplayObjectTransform(Core::Datastructure::Object* object)
	{
		if (ImGui::CollapsingHeader("Transform", m_treeNodeFlags))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, m_isLocal ? ImVec4(0.0f, 0.58f, 0.20f, 1.00f) : ImVec4(0.60f, 0.35f, 0.71f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_isLocal ? ImVec4(0.0f, 0.58f, 0.20f, 0.70f) : ImVec4(0.60f, 0.35f, 0.71f, 0.70f));
			{
				if (ImGui::Button(m_isLocal ? "Local" : "Global", { ImGui::GetContentRegionAvail().x, 0.f }))
					m_isLocal = !m_isLocal;
			}
			ImGui::PopStyleColor(2);

			Core::Maths::Vec3 pos = m_isLocal ? object->GetPos() : object->GetGlobalPos();
			if (ImGui::DragFloat3("Position", pos.xyz))
				m_isLocal ? object->SetPos(pos) : object->SetGlobalPos(pos);

			Core::Maths::Vec3 rot = m_isLocal ? object->GetRot().ToEulerAngles() : object->GetGlobalRot().ToEulerAngles();
			rot *= RAD2DEG;
			if (ImGui::DragFloat3("Rotation", rot.xyz))
				m_isLocal ? object->SetRot(rot * DEG2RAD) : object->SetGlobalRot(rot * DEG2RAD);

			Core::Maths::Vec3 scale = m_isLocal ? object->GetScale() : object->GetGlobalScale();
			if (ImGui::DragFloat3("Scale", scale.xyz, 0.01f))
				m_isLocal ? object->SetScale(scale) : object->SetGlobalScale(scale);
		}
	}

	void WindowInspector::Tick()
	{
		Core::Datastructure::Object* gO = GetEngine()->objectSelected;
		if (gO == nullptr)
			return;

		DisplayObjectName(gO);
		ImGui::Separator();
		DisplayObjectTransform(gO);
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Properties", m_treeNodeFlags))
		{
			for (auto it = gO->GetComponents().begin(); it != gO->GetComponents().end(); ++it)
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
