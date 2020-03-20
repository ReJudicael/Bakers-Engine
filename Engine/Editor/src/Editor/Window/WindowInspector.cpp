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
		auto root = GetEngine()->m_root;
		
		for (auto it = root->GetComponents().begin(); it != root->GetComponents().end(); ++it)
		{
			type t = (*it)->get_type();
			for (auto& prop : t.get_properties())
				ImGui::Text("type: %s\nname: %s\nvalue: %s\n\n",
					prop.get_type().get_name().to_string().c_str(),
					prop.get_name().to_string().c_str(),
					prop.get_value(*it).to_string().c_str());
		}
	}
}
