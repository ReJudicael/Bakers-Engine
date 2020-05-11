#include "WindowFileBrowser.h"
#include "EditorEngine.h"
#include "LoadResources.h"
#include "ScriptedComponent.h"

#include "IconsFontAwesome5.h"

namespace Editor::Window
{
	/**
	 * Excluded extensions
	 */
	std::vector<std::string> excludedExtensions =
	{
		".exe",
		".dll",
		".bat",
		".ini",
		".lib",
		".pdb"
		".vcxproj",
	};

	WindowFileBrowser::WindowFileBrowser(Canvas* canvas, bool visible) :
		AWindow{ canvas, "File Browser", visible }
	{
		m_fs = new Core::SystemManagement::FileSystem();
		m_inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
		m_treeNodeFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}

	WindowFileBrowser::~WindowFileBrowser()
	{
		delete m_fs;
	}

	void WindowFileBrowser::PushWindowStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_Button,			{ 0.f, 0.f, 0.f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	{ 0.259f, 0.588f, 0.980f, 0.392f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,	{ 0.059f, 0.529f, 0.980f, 0.392f });
	}

	void WindowFileBrowser::PopWindowStyle()
	{
		ImGui::PopStyleColor(3);
	}

	bool WindowFileBrowser::CanRename()
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

	void WindowFileBrowser::ApplyNameToItem(const std::string& itemName)
	{
		m_fs->RenameContent(m_fs->GetLocalAbsolute(itemName), m_name);
		m_renamePath.clear();
		m_scrollSetted	= false;
		m_canRename		= false;
	}

	void WindowFileBrowser::RenameContent(const std::string& itemName)
	{
		if (!CanRename())
			return;

		if (strncmp(m_name, itemName.c_str(), itemName.size() + 1) != 0)
			memcpy(m_name, itemName.c_str(), itemName.size() + 1);

		ImGui::SetNextItemWidth(m_contentPathSize - 10.f);
		bool apply = ImGui::InputText("## RenameContent", m_name, IM_ARRAYSIZE(m_name), m_inputTextFlags);

		if (apply || ImGui::IsItemDeactivated())
			ApplyNameToItem(itemName);
	}

	void WindowFileBrowser::MenuItemRefresh()
	{
		if (ImGui::MenuItem("Refresh"))
			m_fs->RefreshCurrentPath();
	}

	void WindowFileBrowser::MenuItemNew()
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Folder"))
				m_renamePath = m_fs->CreateFolder();

			if (ImGui::MenuItem("File"))
				m_renamePath = m_fs->CreateFile();

			ImGui::Separator();

			if (ImGui::MenuItem("Script"))
			{
				const std::string& path = m_fs->CreateFile("Script", ".lua");
				Core::Datastructure::ScriptedComponent::CreateScript(path);
				m_renamePath = path;
			}

			ImGui::EndMenu();
		}
	}

	void WindowFileBrowser::PopupMenuOnWindow()
	{
		if (ImGui::BeginPopupContextWindow("## PopupMenuOnWindow", ImGuiMouseButton_Right, false))
		{
			if (ImGui::MenuItem("Show in Explorer"))
				m_fs->ShowCurrentPathInExplorer();

			ImGui::Separator();
			MenuItemRefresh();
			ImGui::Separator();
			MenuItemNew();
			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::PopupMenuOnItem(const std::string& itemPath)
	{
		if (ImGui::BeginPopupContextItem("## PopupMenuOnItem", ImGuiMouseButton_Right))
		{
			if (ImGui::MenuItem("Open"))
				m_fs->OpenContent(itemPath);

			if (ImGui::MenuItem("Show in Explorer"))
				m_fs->ShowPathInExplorer(itemPath);

			if (itemPath != "..")
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Rename"))
					m_renamePath = itemPath;

				if (ImGui::MenuItem("Delete"))
					m_fs->DeleteContent(itemPath);
			}
			ImGui::Separator();
			MenuItemRefresh();

			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::ZoomPathContents()
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150.f);
		ImGui::SetNextItemWidth(100.f);

		if (ImGui::SliderInt("Size", &m_size, 1, 100))
			m_contentPathSize = 65.f + m_size * 1.3f;
	}

	GLuint WindowFileBrowser::GetIcon(const std::string& itemPath)
	{
		auto it{ m_icons.find(itemPath) };
		if (it == m_icons.end())
		{
			std::string ext;
			if (itemPath == "..")
				ext = "backFolder";
			else if (m_fs->IsDirectory(itemPath))
				ext = "folder";
			else
				ext = m_fs->GetExtensionWithoutDot(itemPath);

			const std::string iconsPath{ "Resources\\Images\\FileBrowserIcons\\icon_" + ext + ".png" };
			std::shared_ptr<Resources::Texture> icon;
			if (m_fs->Exists(iconsPath))
				GetEngine()->GetResourcesManager()->LoadTexture(iconsPath, icon);
			else
				GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\FileBrowserIcons\\icon_default.png", icon);

			it = m_icons.emplace(itemPath, icon).first;
		}

		return it->second->texture;
	}

	void WindowFileBrowser::ShowCurrentPathOnHeader()
	{
		const std::string rootDirectory{ "." };
		if (ImGui::ImageButtonUV(GetIcon(rootDirectory)))
			m_fs->SetCurrentDirectory(rootDirectory);
		DragDropTargetItem(rootDirectory);

		const std::vector<std::string>& foldersPath{ m_fs->GetExplodedCurrentPath() };
		for (int i{ 0 }; i < foldersPath.size(); ++i)
		{
			if (foldersPath[i] == ".")
				continue;

			const size_t& pos{ m_fs->GetCurrentDirectory().find(foldersPath[i]) + foldersPath[i].size() };

			ImGui::SameLine();
			ImGui::PushID(i);
			{
				if (ImGui::Button(foldersPath[i].c_str()))
					m_fs->SetCurrentDirectory(m_fs->GetCurrentPathAtPos(pos));
				DragDropTargetItem(m_fs->GetCurrentPathAtPos(pos));
			}
			ImGui::PopID();

			if (i < foldersPath.size() - 1)
			{
				ImGui::SameLine();
				ImGui::Text(">");
			}
		}

		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 175.f);
		m_pathFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - 37.f);
	}

	void WindowFileBrowser::DragDropSourceItem(const std::string& itemName, const std::string& itemPath)
	{
		if (itemName != ".." && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::ImageUV(GetIcon(itemPath));
			ImGui::SameLine();
			ImGui::Text(itemName.c_str());
			ImGui::SetDragDropPayload("DRAGDROP_PATH", itemPath.c_str(), itemPath.size() + 1, ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}

	void WindowFileBrowser::DragDropTargetItem(const std::string& itemPath)
	{
		if (ImGui::BeginDragDropTarget() && m_fs->IsDirectory(itemPath))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGDROP_PATH", ImGuiDragDropFlags_SourceAllowNullID))
			{
				const char* path{ reinterpret_cast<const char*>(payload->Data) };

				m_fs->MovePath(path, itemPath.c_str());
				ImGui::EndDragDropTarget();
			}
		}
	}

	void WindowFileBrowser::ShowItem(const std::string& itemName)
	{
		const std::string& itemPath = m_fs->GetLocalAbsolute(itemName);
		ImGui::BeginGroup();
		{
			ImGui::ImageButtonUV(GetIcon(itemPath), { m_contentPathSize - 20.f });
			if (m_renamePath == itemPath)
				RenameContent(itemName);
			else
				ImGui::TextWrapped(itemName.c_str());
		}
		ImGui::EndGroup();

		DragDropSourceItem(itemName, itemPath);
		DragDropTargetItem(itemPath);

		PopupMenuOnItem(itemPath);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			m_fs->OpenContent(itemPath);
	}

	void WindowFileBrowser::ShowDirectoryContent(std::vector<std::filesystem::path> contents)
	{
		size_t nbItems{ 0 };
		ImVec2 regionSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30.f };
		ImGui::BeginChild("## ShowDirectoryContent", regionSize, false);
		{
			if (m_fs->GetCurrentDirectory() != ".")
				contents.insert(contents.begin(), "..");

			PopupMenuOnWindow();

			int columns{ static_cast<int>(regionSize.x / m_contentPathSize) };
			if (columns < 1) columns = 1;

			if (ImGui::BeginTable("## ShowDirectoryContentColumns", columns))
			{
				std::string itemName;
				nbItems = 0;
				for (size_t i{ 0 }; i < contents.size(); ++i)
				{
					if (m_fs->IsRefreshedCurrentPath())
					{
						m_icons.clear();
						break;
					}

					itemName = contents[i].filename().string();
					if (itemName != ".." && (m_fs->FileHasExcludedExtension(itemName, excludedExtensions) ||
						!m_pathFilter.PassFilter(itemName.c_str())))
						continue;

					ImGui::TableNextCell();
					ImGui::PushID(static_cast<int>(i));
					ShowItem(itemName);
					ImGui::PopID();
					if (itemName != "..")
						++nbItems;
				}
				ImGui::EndTable();
			}
			ImGui::EndChild();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("%d Items", nbItems);
		ImGui::SameLine();
		ZoomPathContents();
	}

	void WindowFileBrowser::ShowAllFoldersRecursive(std::filesystem::path path)
	{
		for (auto& p : std::filesystem::directory_iterator(path))
		{
			if (m_fs->IsDirectory(p))
			{
				ImGuiTreeNodeFlags flags{ m_treeNodeFlags };

				if (m_fs->GetCurrentDirectory() == p.path().string())
					flags |= ImGuiTreeNodeFlags_Selected;

				bool isOpen = ImGui::TreeNodeEx((ICON_FA_FOLDER "  " + p.path().filename().string()).c_str(), flags);

				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					m_fs->SetCurrentDirectory(p.path().string());

				if (isOpen)
				{
					ShowAllFoldersRecursive(p.path());
					ImGui::TreePop();
				}
			}
		}
	}

	void WindowFileBrowser::ShowDirectoryTree(const std::string& path)
	{
			ImVec2 regionSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2 };
			ImGui::BeginChild("## ShowDirectoryTree", regionSize);
			{
				ShowAllFoldersRecursive(path);
			}
			ImGui::EndChild();
	}

	void WindowFileBrowser::Tick()
	{
		m_fs->MoveCurrentToClosestDirectory();
		ShowCurrentPathOnHeader();
		ImGui::Separator();

		if (ImGui::BeginTable("## FileBrowser", 2, ImGuiTableFlags_BordersVInner | ImGuiTableFlags_BordersVFullHeight | ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("## FileBrowserTree", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() * 10);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ShowDirectoryTree(".");
			ImGui::TableSetColumnIndex(1);
			ShowDirectoryContent(m_fs->GetContentsInCurrentPath());
			ImGui::EndTable();
		}
	}
}
