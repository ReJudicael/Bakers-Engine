#include "WindowFileBrowser.h"
#include "EditorEngine.h"
#include "LoadResources.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Editor::Window
{
	/**
	 * Excluded extensions
	 */
	std::vector<std::string> excludedExtensions = {
	".exe",
	".dll",
	".bat",
	".ini",
	".layout",
	".ilk",
	".lib",
	".pdb"
	};

	WindowFileBrowser::WindowFileBrowser(Canvas* canvas, bool visible) :
		AWindow{ canvas, "File Browser", visible }
	{
		m_inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
	}

	void WindowFileBrowser::PushWindowStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.259f, 0.588f, 0.980f, 0.f });
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
		fs.RenameContent(fs.GetLocalAbsolute(itemName), m_name);
		m_renamePath.clear();
		m_scrollSetted = false;
		m_canRename = false;
	}

	void WindowFileBrowser::RenameContent(const std::string& itemName)
	{
		if (!CanRename())
			return;

		if (strncmp(m_name, itemName.c_str(), itemName.size() + 1) != 0)
			memcpy(m_name, itemName.c_str(), itemName.size() + 1);

		ImGui::SetNextItemWidth(m_contentPathSize - 10.f);

		bool apply = ImGui::InputText("## InputText", m_name, IM_ARRAYSIZE(m_name), m_inputTextFlags);

		if (apply || ImGui::IsItemDeactivated())
			ApplyNameToItem(itemName);
	}

	void WindowFileBrowser::MenuItemRefresh()
	{
		if (ImGui::MenuItem("Refresh"))
			fs.ActualizeContentsInCurrentPath();
	}

	void WindowFileBrowser::MenuItemNew()
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Folder"))
				m_renamePath = fs.CreateFolder();

			if (ImGui::MenuItem("File"))
				m_renamePath = fs.CreateFile();

			ImGui::EndMenu();
		}
	}

	void WindowFileBrowser::PopupMenuOnWindow()
	{
		if (ImGui::BeginPopupContextWindow("## PopupOnWindow", ImGuiMouseButton_Right, false))
		{
			MenuItemRefresh();
			ImGui::Separator();
			MenuItemNew();
			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::PopupMenuOnItem(const std::string& itemPath)
	{
		if (ImGui::BeginPopupContextItem("## PopupOnItem", ImGuiMouseButton_Right))
		{
			if (ImGui::MenuItem("Open"))
				fs.OpenContent(itemPath);

			if (itemPath != "..")
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Rename"))
					m_renamePath = itemPath;

				if (ImGui::MenuItem("Delete"))
					fs.DeleteContent(itemPath);
			}

			ImGui::Separator();
			MenuItemRefresh();
			ImGui::Separator();
			MenuItemNew();

			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::ZoomPathContents()
	{
		ImGui::Indent(ImGui::GetWindowWidth() - 150.f);
		ImGui::SetNextItemWidth(100.f);

		if (ImGui::SliderInt("Size", &m_size, 1, 100))
			m_contentPathSize = 65.f + m_size * 1.3f;
	}

	GLuint WindowFileBrowser::GetIcon(const std::string& itemPath)
	{
		auto it = m_icons.find(itemPath);
		if (it == m_icons.end())
		{
			std::string ext;
			if (itemPath == "..")
				ext = "backFolder";
			else if (fs.IsDirectory(itemPath))
				ext = "folder";
			else
				ext = fs.GetExtensionWithoutDot_str(itemPath);

			std::string iconsPath{ "Resources\\Images\\FileBrowserIcons\\icon_" + ext + ".png" };
			std::shared_ptr<Resources::Texture> icon;
			if (fs.Exists(iconsPath))
				GetEngine()->GetResourcesManager()->LoadTexture(iconsPath, icon);
			else
				GetEngine()->GetResourcesManager()->LoadTexture("Resources\\Images\\FileBrowserIcons\\icon_default.png", icon);

			it = m_icons.emplace(itemPath, icon).first;
		}
#pragma warning(suppress : 4312)
		return it->second->texture;
	}

	void WindowFileBrowser::ShowCurrentPathOnHeader()
	{
		if (ImGui::ImageButtonUV(GetIcon("."), { 16.f }))
			fs.SetCurrentDirectory(".");

		const std::vector<std::string>& foldersPath{ fs.GetExplodedCurrentPath() };
		for (int i = 0; i < foldersPath.size() && fs.GetCurrentDirectory() != "."; ++i)
		{
			ImGui::SameLine();
			ImGui::PushID(i);
			if (ImGui::Button(foldersPath[i].c_str()))
			{
				std::size_t found{ 0 };
				for (int j{ 0 }; j < i; ++j)
					found += foldersPath[j].size() + 1;	// + 1 for slash between each folders
				found += foldersPath[i].size();			// Remove trailing slash, if existing
				fs.CutCurrentPathAtPos(found);
			}
			ImGui::PopID();

			if (i < foldersPath.size() - 1)
			{
				ImGui::SameLine();
				ImGui::Text(">");
			}
		}
	}

	void WindowFileBrowser::ShowItem(const std::string& itemName)
	{
		const std::string& itemPath = fs.GetLocalAbsolute(itemName);

		ImGui::BeginGroup();
		{
			ImGui::ImageButtonUV(GetIcon(itemPath), { m_contentPathSize - 20.f });
			if (m_renamePath == itemPath)
				RenameContent(itemName);
			else
				ImGui::TextWrapped(itemName.c_str());
		}
		ImGui::EndGroup();

		PopupMenuOnItem(itemPath);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			fs.OpenContent(itemPath);
	}

	void WindowFileBrowser::ShowDirectoryContent(std::vector<std::filesystem::path> contents)
	{
		ImVec2 regionSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30.f };
		ImGui::BeginChild("## WindowFileBrowser", regionSize);
		{
			PopupMenuOnWindow();

			int columns{ static_cast<int>(regionSize.x / m_contentPathSize) };
			if (columns < 1)
				columns = 1;
			ImGui::Columns(columns, (const char*)0, false);

			if (fs.GetCurrentDirectory() != ".")
				contents.insert(contents.begin(), "..");

			std::string itemName;
			for (size_t i{ 0 }; i < contents.size(); ++i)
			{
				if (fs.NeedToActualizeContentsInCurrentPath())
				{
					m_icons.clear();
					break;
				}

				itemName = contents[i].filename().string();
				if (fs.FileHasExcludedExtension(itemName, excludedExtensions))
					continue;

				ImGui::PushID(static_cast<int>(i));
				ShowItem(itemName);
				ImGui::PopID();

				ImGui::NextColumn();
			}
			ImGui::EndChild();
		}
		ZoomPathContents();
	}

	void WindowFileBrowser::ShowFileBrowser(const std::vector<std::filesystem::path>& content)
	{
		ShowCurrentPathOnHeader();
		ImGui::Separator();
		ShowDirectoryContent(content);
	}

	void WindowFileBrowser::Tick()
	{
		fs.MoveCurrentToClosestDirectory();
		ShowFileBrowser(fs.GetContentsInCurrentPath());
	}
}
