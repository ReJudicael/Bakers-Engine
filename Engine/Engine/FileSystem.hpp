#pragma once

#include "StringExtension.hpp"
#include <filesystem>

namespace Core::SystemManagement
{
	class FileSystem final
	{
	private:
		std::string m_currentlyPath{ "." };

	public:
		bool IsRegularFile(const std::filesystem::path& path) const noexcept;
		bool IsRegularFile() const noexcept;

		bool IsDirectory(const std::filesystem::path& path) const noexcept;
		bool IsDirectory() const noexcept;

		bool IsEmpty(const std::filesystem::path& path) const noexcept;
		bool IsEmpty() const noexcept;

		bool PathExists(const std::filesystem::path& path) const noexcept;
		bool PathExists() const noexcept;

		bool IsAbsolute(const std::filesystem::path& path) const noexcept;
		bool IsAbsolute() const noexcept;

		std::filesystem::path GetAbsolute(const std::filesystem::path& path) const noexcept;
		std::filesystem::path GetAbsolute() const noexcept;

		std::string GetExtensionFile(const std::filesystem::path& path) const noexcept;

		void MoveToClosestDirectory() noexcept;
		void GetContents(std::vector<std::filesystem::path>& content) noexcept;
		void GetContents(const std::filesystem::path& path, std::vector<std::filesystem::path>& content) noexcept;
	};

	inline bool FileSystem::IsRegularFile(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::is_regular_file(path);
	}

	inline bool FileSystem::IsRegularFile() const noexcept
	{
		return IsRegularFile(m_currentlyPath);
	}

	inline bool FileSystem::IsDirectory(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::is_directory(path);
	}

	inline bool FileSystem::IsDirectory() const noexcept
	{
		return IsDirectory(m_currentlyPath);
	}

	inline bool FileSystem::IsEmpty(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::is_empty(path);
	}

	inline bool FileSystem::IsEmpty() const noexcept
	{
		return IsEmpty(m_currentlyPath);
	}

	inline bool FileSystem::PathExists(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::exists(path);
	}

	inline bool FileSystem::PathExists() const noexcept
	{
		return PathExists(m_currentlyPath);
	}

	inline bool FileSystem::IsAbsolute(const std::filesystem::path& path) const noexcept
	{
		return path.is_absolute();
	}

	inline bool FileSystem::IsAbsolute() const noexcept
	{
		return IsAbsolute(m_currentlyPath);
	}

	inline std::filesystem::path FileSystem::GetAbsolute(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::absolute(path);
	}

	inline std::filesystem::path FileSystem::GetAbsolute() const noexcept
	{
		return std::filesystem::absolute(m_currentlyPath);
	}

	inline std::string FileSystem::GetExtensionFile(const std::filesystem::path& path) const noexcept
	{
		if (IsRegularFile(path))
		{
			const std::string filename = path.string();
			const size_t dotPos = filename.find_last_of(".");

			if (dotPos == std::string::npos)
				return filename.substr(dotPos);
		}
		return "";
	}

	inline void FileSystem::MoveToClosestDirectory() noexcept
	{
		while (!std::filesystem::is_directory(m_currentlyPath))
		{
			// Try to repair path
			size_t pos{ m_currentlyPath.rfind('\\') };
			if (m_currentlyPath.rfind('\\') == std::string::npos)
				m_currentlyPath = ".";

			m_currentlyPath = m_currentlyPath.substr(0, pos);
		}
	}

	inline void FileSystem::GetContents(std::vector<std::filesystem::path>& content) noexcept
	{
		GetContents(m_currentlyPath, content);
	}

	inline void FileSystem::GetContents(const std::filesystem::path& path, std::vector<std::filesystem::path>& content) noexcept
	{
		int directoryCount{ 0 };
		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				content.insert(std::begin(content) + directoryCount, entry.path());
				++directoryCount;
			}
			else
			{
				content.push_back(entry.path());
			}
		}
	}
}