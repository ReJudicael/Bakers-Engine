#include "WindowInspector.h"
#include "Reflection.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
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
			static Core::Maths::Vec3 pos = selected->GetGlobalPos();
			if (ImGui::DragFloat3("Position", pos.xyz))
				selected->SetPos(pos);
			static Core::Maths::Vec3 rot = selected->GetGlobalRot().ToEulerAngles();
			if (ImGui::DragFloat3("Rotation", rot.xyz))
				selected->SetRot(rot);
			static Core::Maths::Vec3 scale = selected->GetGlobalScale();
			if (ImGui::DragFloat3("Scale", scale.xyz))
				selected->SetScale(scale);
			ImGui::Spacing();
		}
	}

	void WindowInspector::Tick()
	{
		auto root = GetEngine()->selected;

		if (root == nullptr)
			return;

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
