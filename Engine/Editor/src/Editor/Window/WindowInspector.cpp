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
		m_treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
	}

	void WindowInspector::PushWindowStyle()
	{
	}

	void WindowInspector::PopWindowStyle()
	{
	}

	void WindowInspector::DisplayTransform(Core::Datastructure::Object* selected)
	{
		if (ImGui::CollapsingHeader("Transform", m_treeNodeFlags))
		{
			if (ImGui::Button(m_isLocal ? "Local" : "Global", { ImGui::GetContentRegionAvailWidth(), 0.f }))
				m_isLocal = !m_isLocal;

			Core::Maths::Vec3 pos = m_isLocal ? selected->GetPos() : selected->GetGlobalPos();
			if (ImGui::DragFloat3("Position", pos.xyz))
				m_isLocal ? selected->SetPos(pos) : selected->SetPos(pos);

			Core::Maths::Vec3 rot = m_isLocal ? selected->GetRot().ToEulerAngles() : selected->GetGlobalRot().ToEulerAngles();
			rot *= RAD2DEG;
			if (ImGui::DragFloat3("Rotation", rot.xyz))
				selected->SetRot(rot * DEG2RAD);

			Core::Maths::Vec3 scale = m_isLocal ? selected->GetScale() : selected->GetGlobalScale();
			if (ImGui::DragFloat3("Scale", scale.xyz))
				m_isLocal ? selected->SetScale(scale) : selected->SetScale(scale);

			ImGui::Spacing();
		}
	}

	void WindowInspector::Tick()
	{
		auto root = GetEngine()->selected;

		if (root == nullptr)
			return;

		ImGui::Text(root->GetName().c_str());
		ImGui::Separator();
		DisplayTransform(root);

		if (ImGui::CollapsingHeader("Properties", m_treeNodeFlags))
		{
			for (auto it = root->GetComponents().begin(); it != root->GetComponents().end(); ++it)
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
