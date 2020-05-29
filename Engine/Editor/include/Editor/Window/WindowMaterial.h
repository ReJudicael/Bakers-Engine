#pragma once

#include "AWindow.h"

namespace Resources
{
	struct Material;
	struct VariantUniform;
	struct Texture;
}

namespace Editor::Window
{
	/**
	 * Window Material
	 */
	class WindowMaterial : public AWindow
	{
	private:
		/**
		 * Flags for Collapsing Header
		 */
		ImGuiTreeNodeFlags m_treeNodeFlags;

		/**
		 * Inspector material
		 */
		std::shared_ptr<Resources::Material> m_material{ nullptr };

		/**
		 * Name material
		 */
		std::string m_nameMaterial;

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
		WindowMaterial(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowMaterial() = default;

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
		 * Show material shaders
		 */
		void ShowMaterialShaders();

		/**
		 * Drop an item
		 * @param itemPath: Path of the dragged file / folder
		 * @param Texture: Texture to send
		 */
		void DragDropTargetItem(const std::string& itemPath, std::shared_ptr<Resources::Texture>& texture);

		/**
		 * Show Material variant
		 * @param varUniform: VariantUniform of Material
		 */
		void ShowMaterialVariant(Resources::VariantUniform& varUniform);

		/**
		 * Show Material texture
		 * @param texture: Texture of Material
		 */
		void ShowMaterialTexture(std::shared_ptr<Resources::Texture>& texture);

		/**
		 * Display the names and current values of the properties
		 * The user can be modify these values
		 */
		void MaterialInspector(std::shared_ptr<Resources::Material>& material);

		void MaterialSaveDiscardInInspector(std::shared_ptr<Resources::Material>& material);

		/**
		 * Button to lock / unlock the selected material
		 */
		void LockSelectedMaterialButton();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
