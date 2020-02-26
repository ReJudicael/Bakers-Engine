#pragma once

#include "StringExtension.hpp"
#include <filesystem>

#define ARRAY_SIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*_ARR)))

namespace Core::SystemManagement
{
	using Path = std::filesystem::path;

	class FileSystem final
	{
	private:
		std::string m_currentlyDirectory{ "." };

	public:
		bool IsRegularFile(const Path& path) const noexcept;
		bool IsDirectory(const Path& path) const noexcept;
		bool IsEmpty(const Path& path) const noexcept;
		bool Exists(const Path& path) const noexcept;
		bool IsAbsolute(const Path& path) const noexcept;

		std::string GetAbsolute(const Path& path) const noexcept;
		std::string GetExtension(const Path& path) const noexcept;
		std::string GetFilename(const Path& path) const noexcept;
		std::string GetCurrentlyPath() const noexcept;

		std::vector<std::string> GetExplodedPath() const noexcept;
		std::vector<std::string> GetExplodedPath(const std::string& path) const noexcept;

		void CutCurrentPathAtPos(size_t pos) noexcept;
		void OpenFolder(const std::string& path) noexcept;
		void RenameContent(const std::string& itemPath, char newName[64]) noexcept;

		void MoveToClosestDirectory() noexcept;
		std::vector<Path> GetContents() const noexcept;
		std::vector<Path> GetContents(const Path& path) const noexcept;

	};

	inline bool FileSystem::IsRegularFile(const Path& path) const noexcept
	{
		return std::filesystem::is_regular_file(path);
	}

	inline bool FileSystem::IsDirectory(const Path& path) const noexcept
	{
		return std::filesystem::is_directory(path);
	}

	inline bool FileSystem::IsEmpty(const Path& path) const noexcept
	{
		return std::filesystem::is_empty(path);
	}

	inline bool FileSystem::Exists(const Path& path) const noexcept
	{
		return std::filesystem::exists(path);
	}

	inline bool FileSystem::IsAbsolute(const Path& path) const noexcept
	{
		return path.is_absolute();
	}

	inline std::string FileSystem::GetAbsolute(const Path& path) const noexcept
	{
		return std::filesystem::absolute(path).string();
	}

	inline std::string FileSystem::GetExtension(const Path& path) const noexcept
	{
		return path.extension().string();
	}

	inline std::string FileSystem::GetFilename(const Path& path) const noexcept
	{
		return path.filename().string();
	}

	inline std::vector<std::string> FileSystem::GetExplodedPath() const noexcept
	{
		return GetExplodedPath(m_currentlyDirectory);
	}

	inline std::vector<std::string> FileSystem::GetExplodedPath(const std::string& path) const noexcept
	{
		return Ext::Explode(path, '\\');
	}

	inline void FileSystem::CutCurrentPathAtPos(size_t pos) noexcept
	{
		m_currentlyDirectory = m_currentlyDirectory.substr(0, pos);
	}

	inline void FileSystem::OpenFolder(const std::string& path) noexcept
	{
		if (std::filesystem::is_directory(path))
		{
			std::string backDirectory = std::filesystem::path(m_currentlyDirectory).parent_path().string();
			m_currentlyDirectory = (path == "..") ? backDirectory : path;
		}
	}

	inline void FileSystem::RenameContent(const std::string& itemPath, char newName[64]) noexcept
	{
		bool isValidName = true;
		char forbiddenChar[10] = "<>:\"/\\|?*";

		for (int i{ 0 }; i < ARRAY_SIZE(newName) && isValidName; ++i)
		{
			for (int j{ 0 }; j < ARRAY_SIZE(forbiddenChar) - 1; ++j)
			{
				if (newName[i] == forbiddenChar[j])
				{
					isValidName = false;
					break;
				}
			}
		}
		std::string newNamePath = m_currentlyDirectory + "\\" + newName;
		if (isValidName && newName[0] && !std::filesystem::exists(newNamePath))
			std::filesystem::rename(itemPath, newNamePath);
	}

	inline void FileSystem::MoveToClosestDirectory() noexcept
	{
		while (!std::filesystem::is_directory(m_currentlyDirectory))
		{
			// Try to repair path
			size_t pos{ m_currentlyDirectory.rfind('\\') };
			if (pos == std::string::npos)
				m_currentlyDirectory = ".";

			CutCurrentPathAtPos(pos);
		}
	}

	inline std::vector<Path> FileSystem::GetContents() const noexcept
	{
		return GetContents(m_currentlyDirectory);
	}

	inline std::vector<Path> FileSystem::GetContents(const Path& path) const noexcept
	{
		std::vector<Path> contents;
		int directoryCount{ 0 };
		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				contents.insert(std::begin(contents) + directoryCount, entry.path());
				++directoryCount;
			}
			else
			{
				contents.push_back(entry.path());
			}
		}
		return contents;
	}

	inline std::string FileSystem::GetCurrentlyPath() const noexcept
	{
		return m_currentlyDirectory;
	}
}
