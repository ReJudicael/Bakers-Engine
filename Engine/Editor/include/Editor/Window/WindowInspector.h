#pragma once

#include "AWindow.h"

namespace Core::Datastructure
{
	class Object;
}

namespace Editor::Window
{
	/**
	 * Window Inspector
	 */
	class WindowInspector final : public AWindow
	{
	private:
		/**
		 * Flags for Collapsing Header
		 */
		ImGuiTreeNodeFlags m_treeNodeFlags;

	private:
		/**
		 * Object selected in hierarchy
		 */
		Core::Datastructure::Object* m_objectSelectedInHierarchy{ nullptr };

		/**
		 * Whether the user wants to modify the local transform or not
		 */
		bool m_isLocal{ true };

	public:
		/**
		 * Constructor which set title of window ("Inspector")
		 */
		WindowInspector(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowInspector() = default;

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
		 * Display name of selected object in hierarchy
		 */
		void DisplayObjectName();

		/**
		 * Display local transform of selected object in hierarchy
		 */
		void DisplayObjectLocalTransform();

		/**
		 * Display global transform of selected object in hierarchy
		 */
		void DisplayObjectGlobalTransform();

		/**
		 * Display transform of selected object in hierarchy
		 */
		void DisplayObjectTransform();

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
