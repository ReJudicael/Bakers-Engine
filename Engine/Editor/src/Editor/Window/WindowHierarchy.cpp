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
		for (auto gO : object->GetChilds())
		{
			ImGui::PushID(gO);
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
				flags |= gO->GetChilds().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;

				if (GetEngine()->objectSelected == gO)
					flags |= ImGuiTreeNodeFlags_Selected;

				bool isOpen = ImGui::TreeNodeEx(gO, flags, gO->GetName().c_str());

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					GetEngine()->objectSelected = gO;

				if (isOpen)
				{
					ShowChildrenOfObject(gO);
					ImGui::TreePop();
				}
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
