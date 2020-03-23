#include "WindowHierarchy.h"
#include "EditorEngine.h"
#include "RootObject.hpp"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Hierarchy", visible }
	{
	}

	void WindowHierarchy::PushWindowStyle()
	{
	}

	void WindowHierarchy::PopWindowStyle()
	{
	}

	void WindowHierarchy::ShowChildrenOfObject(Core::Datastructure::Object* object)
	{
		int i = 0;
		for (auto o : object->GetChilds())
		{
			ImGui::PushID(o);

			bool isExist = !o->GetChilds().empty();
			bool isOpen = false;

			if (isExist)
			{
				isOpen = ImGui::TreeNode(o, "%s", "");
				ImGui::SameLine();
			}

			if (ImGui::Selectable(o->GetName().c_str()))
			{
				GetEngine()->selected = o;
			}

			if (isOpen)
			{
				ShowChildrenOfObject(o);
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}

	void WindowHierarchy::Tick()
	{
		auto root = GetEngine()->m_root;

		ShowChildrenOfObject(root);
	}
}
