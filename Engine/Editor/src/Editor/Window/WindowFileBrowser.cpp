#include "WindowFileBrowser.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad\glad.h>
#include <imgui\imgui_internal.h>

namespace Editor::Window
{
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

	WindowFileBrowser::WindowFileBrowser() :
		AWindow("File Browser")
	{
	}

	void WindowFileBrowser::PushWidgetStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.259f, 0.588f, 0.980f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.259f, 0.588f, 0.980f, 0.392f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.059f, 0.529f, 0.980f, 0.392f });
	}

	void WindowFileBrowser::PopWidgetStyle()
	{
		ImGui::PopStyleColor(3);
	}

	void WindowFileBrowser::Tick()
	{
		PushWidgetStyle();
		{
			fs.MoveCurrentToClosestDirectory();
			ShowDirectory(fs.GetContentsInCurrentPath());
		}
		PopWidgetStyle();
	}

	void WindowFileBrowser::ShowCurrentLocalPath()
	{
		if (ImGui::ImageButton(GetIcon("."), { 16, 16 }, { 0, 1 }, { 1, 0 }))
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

	void WindowFileBrowser::RenameContent(const std::string& itemPath, const std::string& item)
	{
		char name[64];
		memcpy(name, item.c_str(), item.size() + 1);
		ImGui::SetNextItemWidth(m_contentPathSize - 10);

		if (!m_canRename)
		{
			ImGui::SetScrollHereY();
			ImGui::SetKeyboardFocusHere();
			m_canRename = true;
		}

		bool apply = ImGui::InputText("## InputText", name, IM_ARRAYSIZE(name),
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

		if (apply || ImGui::IsItemDeactivated())
		{
			fs.RenameContent(itemPath, name);
			m_renamePath.clear();
			m_canRename = false;
		}
	}

	void WindowFileBrowser::DirectoryContentActionRight(const std::string& itemPath)
	{
		ImGui::OpenPopupOnItemClick("Menu");
		if (ImGui::BeginPopup("Menu", ImGuiWindowFlags_NoMove))
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
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Folder"))
					m_renamePath = fs.CreateFolder();

				if (ImGui::MenuItem("File"))
					m_renamePath = fs.CreateFile();

				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}

	void WindowFileBrowser::ZoomPathContents()
	{
		ImGui::Indent(ImGui::GetWindowWidth() - 150);
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("Size", &m_size, 1, 100);
		m_contentPathSize = 65.f + m_size * 1.3f;
	}

	void WindowFileBrowser::ShowDirectoryContents(std::vector<std::filesystem::path> contents)
	{
		ImVec2 contentSize{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30.f };
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
		ImGui::BeginChild("##ContentRegion", contentSize, true);
		ImGui::PopStyleVar(1);

		int columns{ (int)(contentSize.x / m_contentPathSize) };
		if (columns < 1)
			columns = 1;
		ImGui::Columns(columns, nullptr, false);

		if (fs.GetCurrentDirectory() != ".")
			contents.insert(contents.begin(), std::filesystem::path(".."));

		std::string item;
		std::string itemPath;
		for (size_t i{ 0 }; i < contents.size(); ++i)
		{
			item = std::filesystem::path(contents[i]).filename().string();
			if (item == ".." || fs.GetCurrentDirectory() == ".")
				itemPath = item;
			else
				itemPath = fs.GetCurrentDirectory() + "\\" + item;

			bool cancel{ false };
			for (const std::string& exclude : excludedExtensions)
			{
				if (item.size() < exclude.size())
					continue;

				const char* c{ &item.c_str()[item.size() - exclude.size()] };
				if (strcmp(c, exclude.c_str()) == 0)
					cancel = true;
			}
			if (cancel)
				continue;

			ImGui::PushID((int)i);
			ImGui::BeginGroup();
			ImGui::ImageButton(GetIcon(itemPath), { m_contentPathSize - 20, m_contentPathSize - 20 }, { 0, 1 }, { 1, 0 });

			bool isEditing = (m_renamePath == itemPath);
			if (isEditing)
				RenameContent(itemPath, item);
			else
				ImGui::TextWrapped(item.c_str());

			ImGui::EndGroup();
			ImGui::NextColumn();

			// if ImageButton is clicked on twice by left mouse button
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && !isEditing)
				fs.OpenContent(itemPath);
			else
				// if ImageButton is clicked on right mouse button
				DirectoryContentActionRight(itemPath);

			ImGui::PopID();
		}
		ImGui::EndChild();
		ZoomPathContents();

	}

	void WindowFileBrowser::ShowDirectory(const std::vector<std::filesystem::path>& content)
	{
		this->ShowCurrentLocalPath();
		ImGui::Separator();
		this->ShowDirectoryContents(content);
	}

	ImTextureID WindowFileBrowser::GetIcon(const std::string& item)
	{
		std::string ext = "default";
		if (fs.IsDirectory(item))
		{
			if (item == "..")
				ext = "backFolder";
			else
				ext = "folder";
		}
		else
			ext = fs.GetExtensionWithoutDot_str(item);

		auto it{ icons.find(ext) };
		if (it == icons.end())
		{
			int w, h, channels;
			stbi_set_flip_vertically_on_load(true);
			std::string s{ std::string(PATH_TO_ICONS) + ext + std::string(".png") };
			unsigned char* image{ stbi_load((s).c_str(), &w, &h, &channels, STBI_rgb_alpha) };
			if (image == nullptr)
			{
				if (ext != "default")
					return GetIcon(".default");
				else
					throw "You can't even load a texture!";
			}

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(image);

			it = icons.insert({ ext, (ImTextureID)texture }).first;
		}

		return it->second;
	}
}
