#include "WindowHierarchy.h"
#include "EditorEngine.h"
#include "RootObject.hpp"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Hierarchy", visible }
	{
		m_rootObject = GetEngine()->m_root;
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

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (!m_canRename)
			{
				ImGui::SetScrollHereY();
				ImGui::SetKeyboardFocusHere();
				m_canRename = true;
			}

			bool apply = ImGui::InputText("## InputText", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);

			if (apply || ImGui::IsItemDeactivated())
			{
				m_objectToRename = nullptr;
				object->SetName(name);
				m_canRename = false;
			}
		}
		ImGui::PopStyleVar(2);
	}

	void WindowHierarchy::CreateObject(Core::Datastructure::Object* parent)
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Empty"))
				m_objectToRename = parent->CreateChild("GameObject", {});

			ImGui::EndMenu();
		}
	}

	void WindowHierarchy::PopupMenuOnWindow(Core::Datastructure::Object* root)
	{
		if (ImGui::BeginPopupContextWindow("## PopupOnWindow", ImGuiMouseButton_Right, false))
		{
			CreateObject(root);
			ImGui::EndPopup();
		}
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
				m_destroyObject = true;
				object->Destroy();
				if (GetEngine()->objectSelected == object || object->IsChild(GetEngine()->objectSelected))
					GetEngine()->objectSelected = nullptr;
			}

			ImGui::Separator();
			CreateObject(object);

			ImGui::EndPopup();
		}
	}

	void WindowHierarchy::ShowChildrenOfObject(Core::Datastructure::Object* parent)
	{
		for (auto object : parent->GetChildren())
		{
			ImGui::PushID(object);
			{
				if (object->IsDestroyed())
					continue;

				ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap };
				flags |= object->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
				if (GetEngine()->objectSelected == object)
					flags |= ImGuiTreeNodeFlags_Selected;

				bool isOpen{ false };
				if (object == m_objectToRename)
				{
					ImVec2 cursorPos = { ImGui::GetCursorPos().x + 20, ImGui::GetCursorPos().y };
					isOpen = ImGui::TreeNodeEx(object, flags, object->GetName().c_str());
					ImGui::SetCursorPos(cursorPos);
					RenameObject(object);
				}
				else
				{
					isOpen = ImGui::TreeNodeEx(object, flags, object->GetName().c_str());
				}

				PopupMenuOnItem(object);

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					GetEngine()->objectSelected = object;

				if (m_destroyObject)
				{
					if (isOpen)
						ImGui::TreePop();
					ImGui::PopID();
					m_destroyObject = false;
					continue;
				}

				if (isOpen)
				{
					ShowChildrenOfObject(object);
					ImGui::TreePop();
				}

				ImGui::PopID();
			}
		}
	}

	void WindowHierarchy::Tick()
	{
		if (ImGui::CollapsingHeader(m_rootObject->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			ShowChildrenOfObject(m_rootObject);

		PopupMenuOnWindow(m_rootObject);
	}
}
