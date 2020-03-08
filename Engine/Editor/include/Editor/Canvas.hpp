#pragma once

#include "IWidget.h"

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>

#include <vector>
#include <memory>

namespace Editor
{
	/**
	 * Anchor (position of widget)
	 */
	enum class EAnchor : unsigned short int
	{
		MIDDLE = 0,
		MIDDLE_LEFT,
		MIDDLE_RIGHT,
		MIDDLE_TOP,
		MIDDLE_BOTTOM,

		TOP,
		TOP_LEFT,
		TOP_RIGHT,
		TOP_TOP,
		TOP_BOTTOM,

		BOTTOM,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		BOTTOM_TOP,
		BOTTOM_BOTTOM,

		LEFT,
		LEFT_LEFT,
		LEFT_RIGHT,
		LEFT_TOP,
		LEFT_BOTTOM,

		RIGHT,
		RIGHT_LEFT,
		RIGHT_RIGHT,
		RIGHT_TOP,
		RIGHT_BOTTOM,

		COUNT
	};

	/**
	 * Container that holds widgets
	 */
	class Canvas final
	{
	private:
		/**
		 * Widgets contained in canvas
		 */
		std::vector<std::pair<std::unique_ptr<Widget::IWidget>, EAnchor>> m_widgets;

		/**
		 * Dockspace flags
		 */
		ImGuiWindowFlags m_dockFlags;

	public:
		/**
		 * Default constructor
		 */
		Canvas();

		/**
		 * Default destructor
		 */
		~Canvas() = default;

		/**
		 * Add widget to canvas
		 * @tparam T: Specific Widget class
		 * @tparam args: Arguments of constructor of given Widget class
		 * @return Name of widget added
		 */
		template<class T>
		std::string AddWidget(EAnchor anchor);

		/**
		 * Remove widget in canvas
		 * @param name: Name of widget to remove
		 */
		void RemoveWidget(std::string name);

		/**
		 * Remove all widgets
		 */
		void RemoveAllWidgets();

	private:
		/**
		 * Push dockspace style
		 */
		void PushDockStyle();

		/**
		 * Pop dockspace style
		 */
		void PopDockStyle();

		/**
		 * Set viewport
		 */
		void SetViewport();

		/**
		 * Build docking widgets
		 */
		void BuildDockWidgets(ImGuiID dock_main_id);

		/**
		 * Build dockspqce
		 */
		void BuildDockspace();

	private:
		/**
		 * Set dockspace
		 */
		void SetDockspace();

	public:
		/**
		 * Display docked widgets in canvas
		 */
		void Draw();

	private:
		/**
		 * Create anchor in dockspace
		 * @param ID: Default anchor ID
		 * @param dir: Position relative to default anchor ID
		 * @return ID of obtained anchor
		 */
		ImGuiID DockBuilderSplitNode(ImGuiID& ID, ImGuiDir_ dir);
	};

	template<class T>
	inline std::string Canvas::AddWidget(EAnchor anchor)
	{
		static_assert(std::is_base_of<Widget::IWidget, T>::value);

		std::unique_ptr<Widget::IWidget> widget = std::make_unique<T>();
		const std::string nameWidget = widget->GetName();
		m_widgets.emplace_back(std::make_pair(std::move(widget), anchor));
		return nameWidget;
	}
}
