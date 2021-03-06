#pragma once

#include <filesystem>
#include <string>

#include "StringExtension.hpp"
#include "ShellFileAPI.h"
#include "CoreMinimal.h"

constexpr const char*	DEFAULT_PATH = ".\\Resources";
constexpr size_t		DEGREE_DEFAULT_PATH = 2;

namespace Core::SystemManagement
{
	using Path = std::filesystem::path;

	/**
	 * Tree directory path
	 * An element of Navigation Pane
	 */
	struct TreeDirectoryPath
	{
		std::string path;
		std::string filename;
		std::vector<TreeDirectoryPath> children;

		TreeDirectoryPath() = default;
		TreeDirectoryPath(const std::string& _path) : path{ _path } {}
		TreeDirectoryPath(const std::string& _path, std::string _filename) : path{ _path }, filename{ _filename } {}

		TreeDirectoryPath&	Back() { return children.back(); }
		void				EmplaceBack(const TreeDirectoryPath& td) { children.emplace_back(td); }
	};

	/**
	 * File handling (creation, destruction, move, rename, etc.) class
	 */
	class FileSystem final
	{
	private:
		/**
		 * Current directory
		 */
		std::string m_currentDirectory{ DEFAULT_PATH };

		/**
		 * Tree Directory Path
		 */
		TreeDirectoryPath m_tdp;

		/**
		 * Actualize the contents in current path
		 */
		bool m_refreshContentsInCurrentPath{ true };

		/**
		 * Actualize the tree directory path
		 */
		bool m_refreshTreeDirectoryPath{ true };

		/**
		 * Last directory saved to get the current path separated by directories
		 */
		std::string m_explodedCurrentPathDirectory;

		/**
		 * Contains all elements inside current directory
		 */
		std::vector<Path> m_contentsInCurrentPath;

		/**
		 * Contains the current path separated by directories
		 */
		std::vector<std::string> m_explodedCurrentPath;

	public:
		/**
		 * Default constructor
		 */
		FileSystem() = default;

		/**
		 * Default destructor
		 */
		~FileSystem() = default;

		/**
		 * Get current directory
		 * @return Current directory
		 */
		std::string GetCurrentDirectory() const noexcept;

		/**
		 * Set current directory
		 * @param dir: Directory wanted
		 */
		void SetCurrentDirectory(const std::string& dir) noexcept;

		/**
		 * Actualize contents in current path
		 */
		void RefreshCurrentPath() noexcept;

		/**
		 * Whether the current path is refreshed or not
		 * @return True if the current path is refreshed or not, false otherwise
		 */
		bool IsRefreshedCurrentPath() const noexcept;

		/**
		 * Actualize the tree directory path
		 */
		void RefreshTreeDirectoryPath() noexcept;

		/**
		 * Whether the tree directory path is refreshed or not
		 * @return True if the tree directory path is refreshed or not, false otherwise
		 */
		bool IsRefreshedTreeDirectoryPath() const noexcept;

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
		 * Whether the file has an excluded extension or not
		 * @param fileName: File name
		 * @param excludedExtensions: Excluded extensions
		 * @return True if the file has an excluded extension, false otherwise
		 */
		bool FileHasExcludedExtension(const std::string& fileName, const std::vector<std::string>& excludedExtensions) const noexcept;

		/**
		 * Check if the path is absolute
		 * @param path: Path of the file or directory to check
		 * @return True if the path is absolute, false otherwise
		 */
		bool IsAbsolute(const Path& path) const noexcept;

		/**
		 * Get the absolute file of the given path
		 * @param path: Path to be converted into absolute path
		 * @return Absolute path
		 */
		std::string GetAbsolute(const Path& path) const noexcept;

		/**
		 * Get the absolute file of the given path
		 * @param path: Path to be converted into absolute path
		 * @return Absolute path with quote
		 */
		std::string GetAbsoluteWithQuote(const Path& path) const noexcept;

		/**
		 * Get the local absolute file of the given path
		 * @param path: Path to be converted into local absolute path
		 * @return Local absolute path
		 */
		std::string GetLocalAbsolute(const std::string& itemPath) const noexcept;

		/**
		 * Get the name of the file at the given path
		 * @param path: Path to be the file from which the name is get
		 * @return Name of the file
		 */
		std::string GetFilename(const Path& path) const noexcept;

		/**
		 * Get the name of the file at the given path
		 * @param path: Path to be the file from which the name is get
		 */
		std::string GetFilenameStr(const std::string& fileName) const noexcept;

		/**
		 * Get the extension of the given path
		 * @param path: Path to the file from which the extension is get
		 * @return Extension of the given file with a dot
		 */
		std::string GetExtension(const Path& path) const noexcept;

		/**
		 * Get the extension of the given file
		 * @param itemPath: Path to the file from which the extension is get
		 * @return Extension of the given file without a dot
		 */
		std::string GetExtensionWithoutDotStr(const std::string& itemPath) const noexcept;

		/**
		 * Whether the extension is an image extension or not
		 * @param extensionWithoutDot: Extension without dot
		 * @return True if the extension is an image extension, false otherwise
		 */
		bool IsImageExtension(const std::string& extWithoutDot) const noexcept;

		/**
		 * Whether the extension is an object extension or not
		 * @param extensionWithoutDot: Extension without dot
		 * @return True if the extension is an object extension, false otherwise
		 */
		bool IsObjectExtension(const std::string& extWithoutDot) const noexcept;

		/**
		 * Get the parent directory of the given path
		 * @param path: Path to get the parent directory from
		 * @return Parent directory from given path
		 */
		std::string GetParentPath(const Path& path) const noexcept;

		/**
		 * Get the parent directory of the current path
		 * @return Parent directory from current path
		 */
		std::string GetParentCurrentPath() const noexcept;

		/**
		 * Get the current path separated by directories
		 * @param path: Path to be exploded
		 * @return Vector of strings containing each directory of the current path
		 */
		std::vector<std::string> GetExplodedPath(const std::string& pathStr) const noexcept;

		/**
		 * Get the current path separated by directories
		 * @return Vector of strings containing each directory of the current path
		 */
		std::vector<std::string> GetExplodedCurrentPath() noexcept;

		/**
		 * Get substring from beginning of current path to given pos
		 * @param pos: Index of the character used to separate current Path
		 * @return Current path at pos
		 */
		std::string GetCurrentPathAtPos(size_t pos) const noexcept;

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
		 * Open path in explorer with a windows command
		 * @param filePath: File path
		 **/
		void ShowPathInExplorer(const std::string& itemPath) noexcept;

		/**
		 * Open current path in explorer
		 */
		void ShowCurrentPathInExplorer() noexcept;

		/**
		 * Create a folder
		 * @return path of the new folder
		 */
		std::string CreateFolder(const std::string& folderName = "New Folder") noexcept;

		/**
		 * Create a file with a windows command
		 * @param filePath: File path
		 */
		void CreateFileCMD(const std::string& filePath) noexcept;

		/**
		 * Create a file
		 * @param fileNameWithoutExt: File name without extension
		 * @param extWithDot: File extension with dot
		 * @return Path of the new file
		 */
		std::string CreateFile(const std::string& fileNameWithoutExt = "New File", const std::string& extWithoutDot = "") noexcept;

		/**
		 * Rename file or directory
		 * @param itemPath: Path to the file or directory to rename
		 * @param newName: New name for the given file or directory
		 * @return True if the file was renamed, false otherwise
		 */
		bool RenameContent(const std::string& itemPath, char newName[64]) noexcept;

		/**
		 * Delete file or directory
		 * @param itemPath: Path to the element to delete
		 */
		void DeleteContent(const std::string& itemPath) noexcept;

		/**
		 * Move current path to closest available directory
		 */
		void MoveCurrentToClosestDirectory() noexcept;

		/**
		 * Get all elements inside given directory
		 * @return vector storing each files and directories inside given directory
		 */
		std::vector<Path> GetContentsInPath(const Path& path) const noexcept;

		/**
		 * Get all elements inside current directory
		 * @return vector storing each files and directories inside current directory
		 */
		std::vector<Path> GetContentsInCurrentPath() noexcept;

		/**
		 * Set value in TreeDirectoryPath
		 * @param tdp: Tree directory path
		 */
		void SetDirectoriesInTreeDirectoryPathRecursive(TreeDirectoryPath& tdp) noexcept;

		/*
		 * Get TreeDirectoryPath
		 * @return TreeDirectoryPath
		 */
		TreeDirectoryPath GetTreeDirectoryPath() noexcept;

		/**
		 * Move path
		 * @param oldPath: Old path
		 * @param newPath: New path
		 */
		void MovePath(const std::string& oldPath, const std::string& newPath) noexcept;
	};

	inline std::string FileSystem::GetCurrentDirectory() const noexcept
	{
		return m_currentDirectory;
	}

	inline void FileSystem::SetCurrentDirectory(const std::string& dir) noexcept
	{
		m_currentDirectory = dir;
		m_refreshContentsInCurrentPath = true;
	}

	inline void FileSystem::RefreshCurrentPath() noexcept
	{
		m_refreshContentsInCurrentPath = true;
	}

	inline bool FileSystem::IsRefreshedCurrentPath() const noexcept
	{
		return m_refreshContentsInCurrentPath;
	}

	inline void FileSystem::RefreshTreeDirectoryPath() noexcept
	{
		m_refreshTreeDirectoryPath = true;
	}

	inline bool FileSystem::IsRefreshedTreeDirectoryPath() const noexcept
	{
		return m_refreshTreeDirectoryPath;
	}

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

	inline bool FileSystem::FileHasExcludedExtension(const std::string& fileName, const std::vector<std::string>& excludedExtensions) const noexcept
	{
		for (const std::string& exclude : excludedExtensions)
		{
			if (fileName.size() < exclude.size())
				continue;

			const char* c{ &fileName.c_str()[fileName.size() - exclude.size()] };
			if (strcmp(c, exclude.c_str()) == 0)
				return true;
		}
		return false;
	}

	inline bool FileSystem::IsAbsolute(const Path& path) const noexcept
	{
		return path.is_absolute();
	}

	inline std::string FileSystem::GetAbsolute(const Path& path) const noexcept
	{
		return std::filesystem::absolute(path).string();
	}

	inline std::string FileSystem::GetAbsoluteWithQuote(const Path& path) const noexcept
	{
		return "\"" + std::filesystem::absolute(path).string() + "\"";
	}

	inline std::string FileSystem::GetLocalAbsolute(const std::string& itemPath) const noexcept
	{
		return itemPath == ".." ? itemPath : m_currentDirectory + '\\' + itemPath;
	}

	inline std::string FileSystem::GetFilename(const Path& path) const noexcept
	{
		return path.filename().string();
	}

	inline std::string FileSystem::GetFilenameStr(const std::string& fileName) const noexcept
	{
		return fileName.substr(fileName.find_last_of('\\') + 1, fileName.size());
	}

	inline std::string FileSystem::GetExtension(const Path& path) const noexcept
	{
		return path.extension().string();
	}

	inline std::string FileSystem::GetExtensionWithoutDotStr(const std::string& itemPath) const noexcept
	{
		size_t offset{ itemPath.find_last_of(".") };
		return itemPath.substr(offset + 1, itemPath.size() - offset);
	}

	inline bool FileSystem::IsImageExtension(const std::string& extWithoutDot) const noexcept
	{
		return extWithoutDot == "jpeg" || extWithoutDot == "jpg" || extWithoutDot == "png" ||
			extWithoutDot == "bmp";
	}

	inline bool FileSystem::IsObjectExtension(const std::string& extWithoutDot) const noexcept
	{
		return extWithoutDot == "obj" || extWithoutDot == "fbx" || extWithoutDot == "dae";
	}

	inline std::string FileSystem::GetParentPath(const Path& path) const noexcept
	{
		const std::string& parent_path = path.parent_path().string();
		return parent_path != "" ? parent_path : DEFAULT_PATH;
	}

	inline std::string FileSystem::GetParentCurrentPath() const noexcept
	{
		return GetParentPath(m_currentDirectory);
	}

	inline std::vector<std::string> FileSystem::GetExplodedPath(const std::string& pathStr) const noexcept
	{
		return Ext::Explode(pathStr, '\\');
	}

	inline std::vector<std::string> FileSystem::GetExplodedCurrentPath() noexcept
	{
		if (m_explodedCurrentPathDirectory != m_currentDirectory)
		{
			m_explodedCurrentPath = GetExplodedPath(m_currentDirectory);
			m_explodedCurrentPathDirectory = m_currentDirectory;
		}

		return m_explodedCurrentPath;
	}

	inline std::string FileSystem::GetCurrentPathAtPos(size_t pos) const noexcept
	{
		return m_currentDirectory.substr(0, pos);
	}

	inline bool FileSystem::OpenFolder(const std::string& itemPath) noexcept
	{
		if (IsDirectory(itemPath))
		{
			SetCurrentDirectory((itemPath == "..") ? GetParentPath(m_currentDirectory) : itemPath);
			return true;
		}
		return false;
	}

	inline bool FileSystem::OpenFile(const std::string& itemPath) noexcept
	{
		if (IsRegularFile(itemPath))
		{
			ShellFileAPI::OpenSpecifiedFile(GetAbsoluteWithQuote(itemPath));
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

	inline void FileSystem::ShowPathInExplorer(const std::string& itemPath) noexcept
	{
		ShellFileAPI::OpenSpecifiedLink(GetAbsoluteWithQuote(IsDirectory(itemPath) ? itemPath : GetParentPath(itemPath)));
	}

	inline void FileSystem::ShowCurrentPathInExplorer() noexcept
	{
		ShellFileAPI::OpenSpecifiedLink(m_currentDirectory);
	}

	inline std::string FileSystem::CreateFolder(const std::string& folderName) noexcept
	{
		std::string folderPath = GetLocalAbsolute(folderName);
		if (!Exists(folderPath))
		{
			std::filesystem::create_directory(folderPath);
		}
		else
		{
			for (int i{ 0 }; true; ++i)
			{
				std::string nameNewFolder = folderName + " (" + std::to_string(i) + ")";
				folderPath = GetLocalAbsolute(nameNewFolder);
				if (!Exists(folderPath))
				{
					std::filesystem::create_directory(folderPath);
					break;
				}
			}
		}
		m_refreshContentsInCurrentPath = true;

		return folderPath;
	}

	inline void FileSystem::CreateFileCMD(const std::string& filePath) noexcept
	{
		const std::string& cmd = "echo off >" + GetAbsoluteWithQuote(filePath);
		system(cmd.c_str());
	}

	inline std::string FileSystem::CreateFile(const std::string& fileNameWithoutExt, const std::string& extWithoutDot) noexcept
	{
		std::string filePath{ GetLocalAbsolute(fileNameWithoutExt + "." + extWithoutDot) };
		if (!Exists(filePath))
		{
			CreateFileCMD(filePath);
		}
		else
		{
			for (int i{ 0 }; true; ++i)
			{
				std::string nameNewFile = fileNameWithoutExt + " (" + std::to_string(i) + ")." + extWithoutDot;
				filePath = GetLocalAbsolute(nameNewFile);

				if (!Exists(filePath))
				{
					CreateFileCMD(filePath);
					break;
				}
			}
		}
		m_refreshContentsInCurrentPath = true;

		return filePath;
	}

	inline bool FileSystem::RenameContent(const std::string& itemPath, char newName[64]) noexcept
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
		const std::string& newNamePath = GetLocalAbsolute(newName);
		if (isValidName && !Exists(newNamePath))
		{
			std::filesystem::rename(itemPath, newNamePath);
			m_refreshContentsInCurrentPath = true;
			return true;
		}

		return false;
	}

	inline void FileSystem::DeleteContent(const std::string& itemPath) noexcept
	{
		if (Exists(itemPath))
		{
			std::filesystem::remove_all(itemPath);
			m_refreshContentsInCurrentPath = true;
		}
	}

	inline void FileSystem::MoveCurrentToClosestDirectory() noexcept
	{
		while (!IsDirectory(m_currentDirectory))
		{
			// Try to repair path
			size_t pos{ m_currentDirectory.rfind('\\') };
			if (pos == std::string::npos)
				SetCurrentDirectory(DEFAULT_PATH);

			SetCurrentDirectory(GetCurrentPathAtPos(pos));
		}
	}

	inline std::vector<Path> FileSystem::GetContentsInPath(const Path& path) const noexcept
	{
		if (!Exists(path))
			return std::vector<Path>();

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

	inline std::vector<Path> FileSystem::GetContentsInCurrentPath() noexcept
	{
		if (m_refreshContentsInCurrentPath)
		{
			m_contentsInCurrentPath = GetContentsInPath(m_currentDirectory);
			m_refreshContentsInCurrentPath = false;
		}
		return m_contentsInCurrentPath;
	}

	inline void FileSystem::SetDirectoriesInTreeDirectoryPathRecursive(TreeDirectoryPath& tdp) noexcept
	{
		if (!Exists(tdp.path))
			return;

		for (std::filesystem::directory_entry p : std::filesystem::directory_iterator(tdp.path))
		{
			if (IsDirectory(p))
			{
				tdp.EmplaceBack({ p.path().string(), p.path().filename().string() });
				SetDirectoriesInTreeDirectoryPathRecursive(tdp.Back());
			}
		}
	}

	inline TreeDirectoryPath FileSystem::GetTreeDirectoryPath() noexcept
	{
		if (m_refreshTreeDirectoryPath)
		{
			TreeDirectoryPath tdp{ DEFAULT_PATH };
			SetDirectoriesInTreeDirectoryPathRecursive(tdp);
			m_tdp = tdp;
			m_refreshTreeDirectoryPath = false;
		}
		return m_tdp;
	}

	inline void FileSystem::MovePath(const std::string& oldPath, const std::string& newPath) noexcept
	{
		std::string _newPath{ newPath };
		if (_newPath == "..")
			_newPath = GetParentCurrentPath().c_str();

		_newPath += '\\' + GetFilename(oldPath);

		if (!Exists(_newPath) && oldPath != newPath)
		{
			std::filesystem::rename(oldPath, _newPath);
			m_refreshContentsInCurrentPath = true;
		}
		else
		{
			BAKERS_LOG_WARNING("The path to the destination already exists");
		}
	}
}
