#pragma once
#include "AWindow.h"

namespace Resources
{
	class Shader;
	struct Texture;
}

namespace Editor::Window
{
	class WindowShader : public AWindow
	{
	private:
		/**
		 * Flags for Collapsing Header
		 */
		ImGuiTreeNodeFlags m_treeNodeFlags;

		/**
		 * Inspector material
		 */
		std::shared_ptr<Resources::Shader> m_shader{ nullptr };

		/**
		 * Name material
		 */
		std::string m_nameShader;

		/**
		 * Property icon
		 */
		std::shared_ptr<Resources::Texture> m_propertyIcon;

		/**
		 * Wheter the material is locked or not
		 */
		bool m_isLocked{ false };

	public:
		/**
		 * Constructor which set title of window ("Material")
		 */
		WindowShader(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowShader() = default;

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;

	private:

		/**
		 * Drop an item
		 * @param itemPath: Path of the dragged file / folder
		 * @param Texture: Texture to send
		 */
		void DragDropTargetItem(const std::string & itemPath, std::shared_ptr<Resources::Texture> & texture);

		/**
		 * Display the names and current values of the properties
		 * The user can be modify these values
		 */
		void ShaderInspector(std::shared_ptr<Resources::Shader>& shader);

		/**
		 * Display the vertex or the frament shader path
		 * @param shader: the shader of the WindowShader
		 * @param isVertex: if true diplay the vertex shader path 
		 * else display the fragment shader path 
		 */
		void DisplayVertAndFragShader(std::shared_ptr<Resources::Shader>& shader, bool isVertex);

		/**
		 * Button to lock / unlock the selected material
		 */
		void LockSelectedShaderButton();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
	
}

