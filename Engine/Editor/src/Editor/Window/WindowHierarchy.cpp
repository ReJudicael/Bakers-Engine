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

	void WindowHierarchy::PopupMenu(Core::Datastructure::Object* object)
	{
		ImGui::OpenPopupOnItemClick("WindowHierarchy");
		if (ImGui::BeginPopup("WindowHierarchy", ImGuiWindowFlags_NoMove))
		{
			if (ImGui::MenuItem("Rename"))
			{
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Create"))
			{
				object->CreateChild("Empty", {});
			}

			if (ImGui::MenuItem("Destroy"))
			{
				m_destroySelected = true;
				object->Destroy();
				if (GetEngine()->objectSelected == object)
					GetEngine()->objectSelected = nullptr;
			}
			ImGui::EndPopup();
		}
	}

	void WindowHierarchy::ShowChildrenOfObject(Core::Datastructure::Object* object)
	{
		for (auto gO : object->GetChilds())
		{
			ImGui::PushID(gO);
			{
				if (gO->IsDestroyed())
					continue;

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
				flags |= gO->GetChilds().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;

				if (GetEngine()->objectSelected == gO)
					flags |= ImGuiTreeNodeFlags_Selected;

				bool isOpen = ImGui::TreeNodeEx(gO, flags, gO->GetName().c_str());

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					GetEngine()->objectSelected = gO;
				else
					PopupMenu(gO);

				if (m_destroySelected)
				{
					if (isOpen)
						ImGui::TreePop();
					ImGui::PopID();
					m_destroySelected = false;
					continue;
				}

				if (isOpen)
				{
					ShowChildrenOfObject(gO);
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
		}
	}

	void WindowHierarchy::Tick()
	{
		auto root = GetEngine()->m_root;
		if (ImGui::CollapsingHeader(root->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			ShowChildrenOfObject(root);
	}
}
