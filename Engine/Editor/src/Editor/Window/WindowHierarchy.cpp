#include "WindowHierarchy.h"
#include "EditorEngine.h"
#include "RootObject.hpp"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	WindowHierarchy::WindowHierarchy(Canvas* canvas, bool visible) :
		AWindow{ canvas, "Hierarchy", visible }
	{
		m_treeNodeFlags =	ImGuiTreeNodeFlags_OpenOnDoubleClick	|
							ImGuiTreeNodeFlags_SpanAvailWidth;

		m_inputTextFlags =	ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
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
		bool apply = ImGui::InputText("## RenameObject", m_name, IM_ARRAYSIZE(m_name), m_inputTextFlags);
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
		if (ImGui::BeginPopupContextWindow("## PopupMenuOnWindow", ImGuiMouseButton_Right, false))
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
				m_objectToRename = object;

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
#pragma warning(suppress : 26812)
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
			if (m_objectToRename && object->HasChild(m_objectToRename))
				ImGui::SetNextItemOpen(true);
			isOpen = ImGui::TreeNodeEx(object, flags, (ICON_FA_CUBE "  " + object->GetName()).c_str());
		}

		DragDropSourceItem(object);
		DragDropTargetItem(object);

		PopupMenuOnItem(object);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			GetEngine()->objectSelected = object;

		return isOpen;
	}

	void WindowHierarchy::DragDropSourceItem(Core::Datastructure::Object* object)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::Text((ICON_FA_CUBE "  " + object->GetName()).c_str());
			ImGui::SetDragDropPayload("DRAGDROP_GAMEOBJECT", &object, sizeof(Core::Datastructure::Object*), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}
	void WindowHierarchy::DragDropTargetItem(Core::Datastructure::Object* object)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_GAMEOBJECT"))
			{
				Core::Datastructure::Object* dObject{ *reinterpret_cast<Core::Datastructure::Object**>(payload->Data) };

				if (!dObject->HasChild(object))
					dObject->SetParent(object);
				else
					BAKERS_LOG_WARNING("Hierarchy: Can't set a GameObject in child of child");

				ImGui::EndDragDropTarget();
			}
		}
	}

	void WindowHierarchy::ShowChildrenOfObject(Core::Datastructure::Object* parent)
	{
		for (auto object : parent->GetChildren())
		{
			if (object->IsDestroyed())
				continue;

			ImGui::PushID(object);
			bool isOpen = DrawTreeNodeOfObject(object);
			ImGui::PopID();

			if (m_destroyObject)
			{
				if (isOpen)
					ImGui::TreePop();
				m_destroyObject = false;
				continue;
			}

			if (isOpen)
			{
				ShowChildrenOfObject(object);
				ImGui::TreePop();
			}
		}
	}

	void WindowHierarchy::DisplaySceneHierarchy(Core::Datastructure::RootObject* scene)
	{
		bool isOpen = ImGui::CollapsingHeader(scene->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		DragDropTargetItem(scene);

		if (isOpen)
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
