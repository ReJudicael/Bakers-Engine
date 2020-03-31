#include "WindowHierarchy.h"
#include "EditorEngine.h"
#include "RootObject.hpp"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Hierarchy", visible }
	{
		m_treeNodeFlags	=	ImGuiTreeNodeFlags_OpenOnDoubleClick |
							ImGuiTreeNodeFlags_SpanAvailWidth	 |
							ImGuiTreeNodeFlags_AllowItemOverlap;
	}

	void WindowHierarchy::PushWindowStyle()
	{
	}

	void WindowHierarchy::PopWindowStyle()
	{
	}

	bool WindowHierarchy::CanRename()
	{
		if (!m_scrollSetted)
		{
			ImGui::SetScrollHereY();
			m_scrollSetted = true;
			return false;
		}

		if (m_scrollSetted && !m_canRename)
		{
			ImGui::SetKeyboardFocusHere();
			m_canRename = true;
		}
		return m_canRename;
	}

	void WindowHierarchy::ApplyNameToObject(Core::Datastructure::Object* object)
	{
		object->SetName(m_name);

		m_objectToRename	= nullptr;
		m_canRename			= false;
		m_scrollSetted		= false;
	}

	void WindowHierarchy::RenameObject(Core::Datastructure::Object* object, const ImVec2& cursorPos)
	{
		if (!CanRename())
			return;

		if (strncmp(m_name, object->GetName().c_str(), object->GetName().size() + 1) != 0)
			memcpy(m_name, object->GetName().c_str(), object->GetName().size() + 1);

		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::SetCursorPos(cursorPos);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 3.f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);

		bool apply = ImGui::InputText("## InputText", m_name, IM_ARRAYSIZE(m_name),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

		ImGui::PopStyleVar(2);

		if (apply || ImGui::IsItemDeactivated())
			ApplyNameToObject(object);
	}

	void WindowHierarchy::MenuItemCreate(Core::Datastructure::Object* parent)
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
			MenuItemCreate(root);
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
				if (GetEngine()->objectSelected == object || object->HasChild(GetEngine()->objectSelected))
					GetEngine()->objectSelected = nullptr;
			}
			ImGui::Separator();

			MenuItemCreate(object);

			ImGui::EndPopup();
		}
	}

	bool WindowHierarchy::DrawTreeNodeOfObject(Core::Datastructure::Object* object)
	{
		ImGuiTreeNodeFlags flags{ m_treeNodeFlags };
		flags |= object->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;
		if (GetEngine()->objectSelected == object)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool isOpen{ false };
		if (object == m_objectToRename)
		{
			ImVec2 cursorPos = { ImGui::GetCursorPos().x + 21, ImGui::GetCursorPos().y };
			isOpen = ImGui::TreeNodeEx(object, flags, object->GetName().c_str());
			RenameObject(object, cursorPos);
		}
		else
		{
			if (m_objectToRename != nullptr && object->HasChild(m_objectToRename))
				ImGui::SetNextItemOpen(true);
			isOpen = ImGui::TreeNodeEx(object, flags, object->GetName().c_str());
		}

		return isOpen;
	}

	void WindowHierarchy::ShowChildrenOfObject(Core::Datastructure::Object* parent)
	{
		for (auto object : parent->GetChildren())
		{
			if (object->IsDestroyed())
				continue;

			ImGui::PushID(object);
			{
				bool isOpen = DrawTreeNodeOfObject(object);

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
			}
			ImGui::PopID();
		}
	}

	void WindowHierarchy::DisplaySceneHierarchy(Core::Datastructure::RootObject* scene)
	{
		if (ImGui::CollapsingHeader(scene->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			PopupMenuOnWindow(scene);
			ShowChildrenOfObject(scene);
		}
	}

	void WindowHierarchy::Tick()
	{
		DisplaySceneHierarchy(GetEngine()->m_root);
	}
}
