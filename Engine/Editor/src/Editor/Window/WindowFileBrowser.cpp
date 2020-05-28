#include "WindowFileBrowser.h"
#include "EditorEngine.h"
#include "LoadResources.h"
#include "ScriptedComponent.h"
#include "Object3DGraph.h"

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
		m_treeNodeFlags		= ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		m_inputTextFlags	= ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
	}

	WindowFileBrowser::~WindowFileBrowser()
	{
		delete m_fs;
	}

	void WindowFileBrowser::PushWindowStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.259f, 0.588f, 0.980f, 0.392f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.059f, 0.529f, 0.980f, 0.392f });
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
		if (m_fs->RenameContent(m_fs->GetLocalAbsolute(itemName), m_name))
		{
			RenameMaterial(m_fs->GetLocalAbsolute(itemName));
			RenameShader(m_fs->GetLocalAbsolute(itemName));
		}
		m_renamePath.clear();
		m_scrollSetted = false;
		m_canRename = false;
	}

	void WindowFileBrowser::RenameMaterial(const std::string& itemPath)
	{
		const std::string& newItemPath = m_fs->GetLocalAbsolute(m_name);
		if (m_fs->GetExtensionWithoutDotStr(newItemPath) == "bmat")
			GetEngine()->GetResourcesManager()->ReplaceMaterial(itemPath, newItemPath);
	}

	void WindowFileBrowser::RenameShader(const std::string& itemName)
	{
		const std::string& newItemPath = m_fs->GetLocalAbsolute(m_name);
		if (m_fs->GetExtensionWithoutDotStr(newItemPath) == "bshader")
			GetEngine()->GetResourcesManager()->ReplaceShader(itemName, newItemPath);
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
				const std::string& path = m_fs->CreateFile("Script", "lua");
				Core::Datastructure::ScriptedComponent::CreateScript(path);
				m_renamePath = path;
			}

			if (ImGui::MenuItem("Material"))
			{
				const std::string& path = m_fs->CreateFile("Material", "bmat");
				GetEngine()->GetResourcesManager()->CreateNewMaterial(path);
				m_renamePath = path;
			}

			if (ImGui::MenuItem("Shader"))
			{
				const std::string& path = m_fs->CreateFile("Shader", "bshader");
				GetEngine()->GetResourcesManager()->CreateNewShader(path);
				m_renamePath = path;
			}

			ImGui::EndMenu();
		}
	}

	void WindowFileBrowser::PopupMenuOnDirectoryContentWindow()
	{
		if (ImGui::BeginPopupContextWindow("## PopupMenuOnDirectoryContentWindow", ImGuiMouseButton_Right, false))
		{
			if (ImGui::MenuItem("Show in Explorer"))
				m_fs->ShowCurrentPathInExplorer();

			ImGui::Separator();

			if (ImGui::MenuItem("Refresh"))
				m_fs->RefreshCurrentPath();

			ImGui::Separator();
			MenuItemNew();
			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::PopupMenuOnDirectoryContentItem(const std::string& itemPath)
	{
		if (ImGui::BeginPopupContextItem("## PopupMenuOnDirectoryContentItem", ImGuiMouseButton_Right))
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
				{
					m_fs->DeleteContent(itemPath);
					DeleteSpecialFile(itemPath);
				}
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Refresh"))
				m_fs->RefreshCurrentPath();

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
				ext = m_fs->GetExtensionWithoutDotStr(itemPath);

			std::string iconsPath{ itemPath };
			if (!m_fs->IsImageExtension(ext))
				iconsPath = "Resources\\Images\\FileBrowserIcons\\icon_" + ext + ".png";

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
		const std::string& rootDirectory{ DEFAULT_PATH };
		if (ImGui::ImageButtonUV(GetIcon(rootDirectory)))
			m_fs->SetCurrentDirectory(rootDirectory);
		DragDropTargetItem(rootDirectory);

		const std::vector<std::string>& foldersPath{ m_fs->GetExplodedCurrentPath() };
		for (size_t i{ DEGREE_DEFAULT_PATH }; i < foldersPath.size(); ++i)
		{
			const size_t& pos{ m_fs->GetCurrentDirectory().find(foldersPath[i]) + foldersPath[i].size() };

			ImGui::SameLine();
			ImGui::PushID(static_cast<int>(i));
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
		if (itemName != ".." && !m_canRename && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::ImageUV(GetIcon(itemPath));
			ImGui::SameLine();
			ImGui::Text(itemName.c_str());
			ImGui::SetDragDropPayload("DRAGDROP_PATH",
				itemPath.c_str(), itemPath.size() + 1, ImGuiCond_Once);
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
				if (m_fs->Exists(path))
					m_fs->MovePath(path, itemPath.c_str());
			}
			ImGui::EndDragDropTarget();
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

		PopupMenuOnDirectoryContentItem(itemPath);

		if (ImGui::IsItemClicked() && m_fs->GetExtensionWithoutDotStr(itemPath) == "bmat")
		{
			GetEngine()->materialSelected = GetEngine()->GetResourcesManager()->GetMaterial(itemPath);
			GetEngine()->nameMaterialSelected = itemPath;
		}
		else if(ImGui::IsItemClicked() && m_fs->GetExtensionWithoutDotStr(itemPath) == "bshader")
		{
			GetEngine()->shaderSelected = GetEngine()->GetResourcesManager()->GetShader(itemPath);
			GetEngine()->nameShaderSelected = itemPath;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			m_fs->OpenContent(itemPath);
	}

	void WindowFileBrowser::ShowVirtualMaterial(const std::string& materialPath)
	{
		const unsigned int materialIcon = GetIcon("mat");
		const std::string& fileName = m_fs->GetFilenameStr(materialPath);

		ImGui::BeginGroup();
		{
			ImGui::ImageButtonUV(materialIcon, { m_contentPathSize - 20.f });
			ImGui::TextWrapped(fileName.c_str());
		}
		ImGui::EndGroup();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::ImageUV(materialIcon);
			ImGui::SameLine();
			ImGui::Text(fileName.c_str());
			ImGui::SetDragDropPayload("DRAGDROP_PATH", materialPath.c_str(), materialPath.size() + 1, ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			GetEngine()->materialSelected = GetEngine()->GetResourcesManager()->GetMaterial(materialPath);
			GetEngine()->nameMaterialSelected = materialPath;
		}
	}

	void WindowFileBrowser::ShowDirectoryContent(std::vector<std::filesystem::path> contents)
	{
		size_t nbItems{ 0 };
		ImVec2 regionSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30.f };
		ImGui::BeginChild("## ShowDirectoryContent", regionSize, false);
		{
			if (m_fs->GetCurrentDirectory() != DEFAULT_PATH)
				contents.insert(contents.begin(), "..");

			PopupMenuOnDirectoryContentWindow();

			int columns{ static_cast<int>(regionSize.x / m_contentPathSize) };
			if (columns < 1) columns = 1;

			if (ImGui::BeginTable("## ShowDirectoryContentTable", columns))
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

					itemName = m_fs->GetFilename(contents[i]);

					if (itemName != ".." && (m_fs->FileHasExcludedExtension(itemName, excludedExtensions) ||
						!m_pathFilter.PassFilter(itemName.c_str())))
						continue;

					ImGui::TableNextCell();
					ImGui::PushID(static_cast<int>(i));
					ShowItem(itemName);
					ImGui::PopID();

					if (itemName != "..")
						++nbItems;

					if (m_fs->IsObjectExtension(m_fs->GetExtensionWithoutDotStr(itemName)))
					{
						if (const auto& info = GetEngine()->GetObject3DInfo(m_fs->GetLocalAbsolute(itemName)))
						{
							for (size_t i{ 0 }; i < info->materialsName.size(); ++i)
							{
								ImGui::TableNextCell();
								ImGui::PushID(static_cast<int>(i));
								ShowVirtualMaterial(info->materialsName[i]);
								ImGui::PopID();
							}
						}
					}
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

	void WindowFileBrowser::ShowAllFoldersRecursive(const Core::SystemManagement::TreeDirectoryPath& tdp)
	{
		for (const Core::SystemManagement::TreeDirectoryPath& p : tdp.children)
		{
			ImGuiTreeNodeFlags flags{ m_treeNodeFlags };
#pragma warning(suppress : 26812)
			flags |= p.children.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow;

			if (m_fs->GetCurrentDirectory() == p.path)
				flags |= ImGuiTreeNodeFlags_Selected;

			bool isOpen = ImGui::TreeNodeEx((ICON_FA_FOLDER "  " + p.filename).c_str(), flags);
			DragDropTargetItem(p.path);

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				m_fs->SetCurrentDirectory(p.path);

			if (isOpen)
			{
				ShowAllFoldersRecursive(p);
				ImGui::TreePop();
			}
		}
	}

	void WindowFileBrowser::PopupMenuOnNavigationPanelWindow()
	{
		if (ImGui::BeginPopupContextWindow("## PopupMenuOnNavigationPanelWindow", ImGuiMouseButton_Right))
		{
			if (ImGui::MenuItem("Refresh"))
				m_fs->RefreshTreeDirectoryPath();
			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::ShowNavigationPanel(const Core::SystemManagement::TreeDirectoryPath& tdp)
	{
		ImVec2 regionSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2 };
		ImGui::BeginChild("## ShowDirectoryTree", regionSize);
		{
			PopupMenuOnNavigationPanelWindow();
			ShowAllFoldersRecursive(tdp);
		}
		ImGui::EndChild();
	}

	void WindowFileBrowser::DeleteSpecialFile(const std::string& itemPath)
	{
		GetEngine()->GetResourcesManager()->DeleteSpecialMaterialShader(itemPath);
	}

	void WindowFileBrowser::Tick()
	{
		m_fs->MoveCurrentToClosestDirectory();
		ShowCurrentPathOnHeader();
		ImGui::Separator();

		if (ImGui::BeginTable("## FileBrowser", 2, ImGuiTableFlags_BordersVInner | ImGuiTableFlags_BordersVFullHeight | ImGuiTableFlags_Resizable))
		{
			ImGui::TableSetupColumn("## PanelNavigation", ImGuiTableColumnFlags_WidthFixed, ImGui::GetFontSize() * 10);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ShowNavigationPanel(m_fs->GetTreeDirectoryPath());
			ImGui::TableSetColumnIndex(1);
			ShowDirectoryContent(m_fs->GetContentsInCurrentPath());
			ImGui::EndTable();
		}
	}
}
