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
	std::string excludedExtensions[] = {
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

	void WindowFileBrowser::RenameContent(const std::string& itemName)
	{
		char name[64];
		memcpy(name, itemName.c_str(), itemName.size() + 1);

		ImGui::SetNextItemWidth(m_contentPathSize - 10.f);

		if (!m_scrollSetted)
		{
			ImGui::SetScrollHereY();
			m_scrollSetted = true;
			return;
		}

		if (!m_canRename && m_scrollSetted)
		{
			ImGui::SetKeyboardFocusHere();
			m_canRename = true;
		}

		bool apply = ImGui::InputText("## InputText", name, IM_ARRAYSIZE(name),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

		if (apply || ImGui::IsItemDeactivated())
		{
			fs.RenameContent(fs.GetLocalAbsolute(itemName), name);
			m_renamePath.clear();
			m_canRename = false;
			m_scrollSetted = false;
		}
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
			MenuItemNew();

			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::ZoomPathContents()
	{
		ImGui::Indent(ImGui::GetWindowWidth() - 150.f);
		ImGui::SetNextItemWidth(100.f);
		ImGui::SliderInt("Size", &m_size, 1, 100);

		m_contentPathSize = 65.f + m_size * 1.3f;
	}

	ImTextureID WindowFileBrowser::GetIcon(const std::string& itemPath)
	{
		std::string ext;
		if (fs.IsDirectory(itemPath))
		{
			if (itemPath == "..")
				ext = "backFolder";
			else
				ext = "folder";
		}
		else
		{
			ext = fs.GetExtensionWithoutDot_str(itemPath);
		}

		std::string iconsPath{ PATH_TO_ICONS + ext + ".png" };
		std::shared_ptr<Resources::Texture> icon;
		if (fs.Exists(iconsPath))
			GetEngine()->GetResourcesManager()->LoadTexture(iconsPath, icon);
		else
			return GetIcon(".default");

#pragma warning(suppress : 4312)
		return reinterpret_cast<ImTextureID>(icon->texture);
	}

	void WindowFileBrowser::ShowCurrentPathOnHeader()
	{
		if (ImGui::ImageButton(GetIcon("."), { 16.f, 16.f }, { 0.f, 1.f }, { 1.f, 0.f }))
			fs.SetCurrentDirectory(".");

		std::vector<std::string> foldersPath{ fs.GetExplodedCurrentPath() };
		for (int i = 0; i < foldersPath.size(); ++i)
		{
			if (fs.GetCurrentDirectory() != ".")
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
			}

			if (i < foldersPath.size() - 1)
			{
				ImGui::SameLine();
				ImGui::Text(">");
			}
		}
	}

	bool WindowFileBrowser::FileHasExcludedExtension(const std::string& itemName)
	{
		for (const std::string& exclude : excludedExtensions)
		{
			if (itemName.size() < exclude.size())
				continue;

			const char* c{ &itemName.c_str()[itemName.size() - exclude.size()] };
			if (strcmp(c, exclude.c_str()) == 0)
				return true;
		}
		return false;
	}

	void WindowFileBrowser::ShowItem(const std::string& itemName, const std::string& itemPath)
	{
		ImGui::BeginGroup();

		ImGui::ImageButton(GetIcon(itemPath), { m_contentPathSize - 20.f, m_contentPathSize - 20.f }, { 0.f, 1.f }, { 1.f, 0.f });
		bool isEditing{ m_renamePath == itemPath };
		if (isEditing)
			RenameContent(itemName);
		else
			ImGui::TextWrapped(itemName.c_str());

		ImGui::EndGroup();

		PopupMenuOnItem(itemPath);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !isEditing)
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

			ImGui::Columns(columns, nullptr, false);
			if (fs.GetCurrentDirectory() != ".")
				contents.insert(contents.begin(), std::filesystem::path(".."));

			std::string itemName, itemPath;
			for (size_t i{ 0 }; i < contents.size(); ++i)
			{
				itemName = std::filesystem::path(contents[i]).filename().string();
				itemPath = fs.GetLocalAbsolute(itemName);

				if (FileHasExcludedExtension(itemName) || !fs.Exists(itemPath))
					continue;

				ImGui::PushID(static_cast<int>(i));
				ShowItem(itemName, itemPath);
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
