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
		void DisplayObjectName(Core::Datastructure::Object* object);

		/**
		 * Display transform of selected object in hierarchy
		 */
		void DisplayObjectTransform(Core::Datastructure::Object* object);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
