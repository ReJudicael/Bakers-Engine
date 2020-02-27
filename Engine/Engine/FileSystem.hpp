#pragma once

#include <filesystem>
#include <string>

#include "StringExtension.hpp"

namespace Core::SystemManagement
{
	using Path = std::filesystem::path;

	/**
	 * File handling (creation, destruction, move, rename, etc.) class
	 */
	class FileSystem final
	{
	public:
		std::string currentDirectory{ "." };

		/**
		 * Check if the given path leads to a file
		 * @param path: Path of the file to check
		 * @return True if the path leads to a file, false if it is a directory or anything else
		 */
		bool IsRegularFile(const Path& path) const noexcept;

		/**
		 * Check if the given path leads to a directory
		 * @param path: Path of the directory to check
		 * @return True if the path leads to a directory, false if it is a file or anything else
		*/
		bool IsDirectory(const Path& path) const noexcept;

		/**
		 * Check if the path is empty
		 * @param path: Path of the directory to check
		 * @return True if the path is empty, false otherwise
		 */
		bool IsEmpty(const Path& path) const noexcept;

		/**
		 * Check if the path leads to a file or directory that exists
		 * @param path: Path of the file or directory to check
		 * @return True if the path leads to a file or directory, false otherwise
		 */
		bool Exists(const Path& path) const noexcept;

		/**
		 * Check if the path is absolute
		 * @param path: Path of the file or directory to check
		 * @return True if the path is absolute, false otherwise
		 */
		bool IsAbsolute(const Path& path) const noexcept;

		/**
		 * Get the absolute file of the given path
		 * @param path: Path to be converted into absolute path
		 * @return Absolute path with quote
		 */
		std::string GetAbsoluteWithQuote(const Path& path) const noexcept;

		/**
		 * Get the absolute file of the given path
		 * @param path: Path to be converted into absolute path
		 * @return Absolute path
		 */
		std::string GetAbsolute(const Path& path) const noexcept;

		/**
		 * Get the local absolute file of the given path
		 * @param path: Path to be converted into local absolute path
		 * @return Local absolute path
		 */
		std::string GetLocalAbsolute(const std::string& itemPath) const noexcept;

		/**
		 * Get the extension of the given
		 * @param path: Path to the file from which the extension is get
		 * @return Extension of the given file
		 */
		std::string GetExtension(const Path& path) const noexcept;

		/**
		 * Get the name of the file at the given path
		 * @param path: Path to be the file from which the name is get
		 * @return Name of the file
		 */
		std::string GetFilename(const Path& path) const noexcept;

		/**
		 * Get the parent directory of the current path
		 * @return Parent directory from current path
		 */
		std::string GetParentCurrentPath() const noexcept;

		/**
		 * Get the parent directory of the given path
		 * @param path: Path to get the parent directory from
		 * @return Parent directory from given path
		 */
		std::string GetParentPath(const Path& path) const noexcept;

		/**
		 * Get the current path separated by directories
		 * @return Vector of strings containing each directory of the current path
		 */
		std::vector<std::string> GetExplodedCurrentPath() const noexcept;

		/**
		 * Get the current path separated by directories
		 * @param path: Path to be exploded
		 * @return Vector of strings containing each directory of the current path
		 */
		std::vector<std::string> GetExplodedPath(const std::string& pathStr) const noexcept;

		/**
		 * Cut substring from beginning of current path to given pos
		 * @param pos: Index of the character used to separate current Path
		 */
		void CutCurrentPathAtPos(size_t pos) noexcept;

		/**
		 * Open given folder
		 * @param itemPath: Path to the folder to open;
		 * @return True if the given path leads to a directory
		 */
		bool OpenFolder(const std::string& itemPath) noexcept;

		/**
		 * Open given file
		 * @param itemPath: Path to the file to open;
		 * @return True if the given path leads to a file
		 */
		bool OpenFile(const std::string& itemPath) noexcept;

		/**
		 * Open given file or directory
		 * @param itemPath: Path to the file or directory to open;
		 * @return True if the given path leads to a file or a directory
		 */
		bool OpenContent(const std::string& itemPath) noexcept;

		/**
		 * Rename file or directory
		 * @param itemPath: Path to the file or directory to rename
		 * @param newName: New name for the given file or directory
		 */
		void RenameContent(const std::string& itemPath, char newName[64]) noexcept;

		/**
		 * Delete file or directory
		 * @param itemPath: Path to the element to delete
		 */
		void DeleteContent(const std::string& itemPath) noexcept;

		/**
		 * Create a folder
		 * @return path of the new folder
		 */
		std::string CreateFolder() noexcept;

		/**
		 * Create a file
		 * @return path of the new file
		 */
		std::string CreateFile() noexcept;


		/**
		 * Move current path to closest available directory
		 */
		void MoveCurrentToClosestDirectory() noexcept;

		/**
		 * Get all elements inside current directory
		 * @return vector storing each files and directories inside current directory
		 */
		std::vector<Path> GetContentsInCurrentPath() const noexcept;

		/**
		 * Get all elements inside given directory
		 * @return vector storing each files and directories inside given directory
		 */
		std::vector<Path> GetContentsInPath(const Path& path) const noexcept;
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

	inline std::string FileSystem::GetAbsoluteWithQuote(const Path& path) const noexcept
	{
		return "\"" + std::filesystem::absolute(path).string() + "\"";
	}

	inline std::string FileSystem::GetAbsolute(const Path& path) const noexcept
	{
		return std::filesystem::absolute(path).string();
	}

	inline std::string FileSystem::GetLocalAbsolute(const std::string& itemPath) const noexcept
	{
		return currentDirectory == "." ? itemPath : currentDirectory + '\\' + itemPath;
	}

	inline std::string FileSystem::GetExtension(const Path& path) const noexcept
	{
		return path.extension().string();
	}

	inline std::string FileSystem::GetFilename(const Path& path) const noexcept
	{
		return path.filename().string();
	}

	inline std::string FileSystem::GetParentCurrentPath() const noexcept
	{
		return GetParentPath(currentDirectory);
	}

	inline std::string FileSystem::GetParentPath(const Path& path) const noexcept
	{
		return path.parent_path().string();
	}

	inline std::vector<std::string> FileSystem::GetExplodedCurrentPath() const noexcept
	{
		return GetExplodedPath(currentDirectory);
	}

	inline std::vector<std::string> FileSystem::GetExplodedPath(const std::string& pathStr) const noexcept
	{
		return Ext::Explode(pathStr, '\\');
	}

	inline void FileSystem::CutCurrentPathAtPos(size_t pos) noexcept
	{
		currentDirectory = currentDirectory.substr(0, pos);
	}

	inline bool FileSystem::OpenFolder(const std::string& itemPath) noexcept
	{
		if (IsDirectory(itemPath))
		{
			currentDirectory = (itemPath == "..") ? GetParentPath(currentDirectory) : itemPath;
			return true;
		}
		return false;
	}

	inline bool FileSystem::OpenFile(const std::string& itemPath) noexcept
	{
		if (IsRegularFile(itemPath))
		{
			std::string cmd = "openwith " + GetAbsoluteWithQuote(itemPath);
			system(cmd.c_str());
			return true;
		}
		else
			return false;
	}

	inline bool FileSystem::OpenContent(const std::string& itemPath) noexcept
	{
		if (!OpenFolder(itemPath))
			return OpenFile(itemPath);
		else
			return true;
	}

	inline void FileSystem::RenameContent(const std::string& itemPath, char newName[64]) noexcept
	{
		bool isValidName = true;
		constexpr char forbiddenChar[10] = "<>:\"/\\|?*";

		for (int i{ 0 }; i < sizeof(newName) && isValidName; ++i)
		{
			for (int j{ 0 }; j < sizeof(forbiddenChar) - 1; ++j)
			{
				if (newName[i] == forbiddenChar[j])
				{
					isValidName = false;
					break;
				}
			}
		}
		std::string newNamePath = GetLocalAbsolute(newName);
		if (isValidName && !Exists(newNamePath))
			std::filesystem::rename(itemPath, newNamePath);
	}

	inline std::string FileSystem::CreateFolder() noexcept
	{
		const std::string defaultNameNewFolder = "New Folder";
		std::string folderPath = GetLocalAbsolute(defaultNameNewFolder);
		if (!Exists(folderPath))
		{
			std::filesystem::create_directory(folderPath);
		}
		else
		{
			for (int i = 0; true; ++i)
			{
				std::string nameNewFolder = defaultNameNewFolder + " (" + std::to_string(i) + ")";
				folderPath = GetLocalAbsolute(nameNewFolder);
				if (!Exists(folderPath))
				{
					std::filesystem::create_directory(folderPath);
					break;
				}
			}
		}
		return folderPath;
	}

	inline std::string FileSystem::CreateFile() noexcept
	{
		const std::string defaultNameNewFile = "New File";
		std::string filePath = GetLocalAbsolute(defaultNameNewFile);
		if (!Exists(filePath))
		{
			std::string cmd = "break > " + GetAbsoluteWithQuote(filePath);
			system(cmd.c_str());
		}
		else
		{
			for (int i = 0; true; ++i)
			{
				std::string nameNewFile = defaultNameNewFile + " (" + std::to_string(i) + ")";
				filePath = GetLocalAbsolute(nameNewFile);

				if (!Exists(filePath))
				{
					std::string cmd = "break > " + GetAbsoluteWithQuote(filePath);
					system(cmd.c_str());
					break;
				}
			}
		}
		return filePath;
	}

	inline void FileSystem::DeleteContent(const std::string& itemPath) noexcept
	{
		if (Exists(itemPath))
			std::filesystem::remove_all(itemPath);
	}

	inline void FileSystem::MoveCurrentToClosestDirectory() noexcept
	{
		while (!IsDirectory(currentDirectory))
		{
			// Try to repair path
			size_t pos{ currentDirectory.rfind('\\') };
			if (pos == std::string::npos)
				currentDirectory = ".";

			CutCurrentPathAtPos(pos);
		}
	}

	inline std::vector<Path> FileSystem::GetContentsInCurrentPath() const noexcept
	{
		return GetContentsInPath(currentDirectory);
	}

	inline std::vector<Path> FileSystem::GetContentsInPath(const Path& path) const noexcept
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
}
