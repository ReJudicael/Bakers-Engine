#include "WindowInspector.h"
#include "Reflection.h"
#include "EditorEngine.h"
#include "RootObject.hpp"
#include <iostream>

namespace Editor::Window
{
	WindowInspector::WindowInspector(Canvas* parent, bool visible) :
		AWindow{ parent, "Inspector", visible }
	{
	}

	void WindowInspector::PushWindowStyle()
	{
	}

	void WindowInspector::PopWindowStyle()
	{
	}

	void WindowInspector::Tick()
	{
		auto root = GetEngine()->selected;

		if (root == nullptr)
			return;

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
