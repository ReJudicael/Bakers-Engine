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

	void WindowHierarchy::RenameObject(Core::Datastructure::Object* object)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 3.f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
		{
			char name[64];
			memcpy(name, m_objectToRename->GetName().c_str(), m_objectToRename->GetName().size() + 1);

			ImGui::SetCursorPos(m_cursorPos);
			ImGui::SetNextItemWidth(-FLT_MIN);

			if (!m_canRename)
			{
				ImGui::SetKeyboardFocusHere();
				m_canRename = true;
			}

			bool apply = ImGui::InputText("## InputText", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);

			if (apply || ImGui::IsItemDeactivated())
			{
				m_objectToRename = nullptr;
				object->SetName(name);
				m_canRename = true;
			}
		}
		ImGui::PopStyleVar(2);
	}

	void WindowHierarchy::PopupMenuOnItem(Core::Datastructure::Object* object)
	{
		if (ImGui::BeginPopupContextItem("## PopupMenuOnItem"))
		{
			if (ImGui::MenuItem("Rename"))
			{
				m_objectToRename = object;
			}

			if (ImGui::MenuItem("Delete"))
			{
				m_destroySelected = true;
				object->Destroy();
				if (GetEngine()->objectSelected == object)
					GetEngine()->objectSelected = nullptr;
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty"))
					object->CreateChild("GameObject", {});
				
				ImGui::EndMenu();
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

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
				flags |= gO->GetChilds().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
				if (GetEngine()->objectSelected == gO)
					flags |= ImGuiTreeNodeFlags_Selected;

				bool isOpen;
				if (gO == m_objectToRename)
				{
					m_cursorPos = { ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y };
					isOpen = ImGui::TreeNodeEx(gO, flags, gO->GetName().c_str());
					RenameObject(gO);
				}
				else
					bool isOpen = ImGui::TreeNodeEx(gO, flags, gO->GetName().c_str());

				PopupMenuOnItem(gO);

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					GetEngine()->objectSelected = gO;


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
