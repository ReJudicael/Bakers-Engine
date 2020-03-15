#pragma once

#include "IDrawable.h"
#include "AWindow.h"

#include "MenuGroup.h"
#include "MenuBar.h"

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>

#include <vector>
#include <memory>

namespace Editor
{
	/**
	 * Dockspace handler
	 */
	class Canvas final : public virtual Datastructure::IDrawable
	{
	private:
		/**
		 * Dockspace window flags
		 */
		ImGuiWindowFlags m_dockWindowFlags;

		/**
		 * Dockspace node flags
		 */
		ImGuiDockNodeFlags m_dockNodeFlags;

	private:
		/**
		 * Widgets list
		 */
		std::vector<std::unique_ptr<Window::AWindow>> m_widgets;

		/**
		 * Menu bar to display menu groups and menu items
		 */
		Menu::MenuBar* m_menuBar;

		/**
		 * Menu group "View" to display or not a widget 
		 */
		Menu::MenuGroup* m_view;

	public:
		/**
		 * Default constructor
		 */
		Canvas();

		/**
		 * Default destructor
		 */
		~Canvas() = default;

	public:
		/**
		 * Initialize Menur Bar
		 */
		void InitMenuBar();

		/**
		 * Add widget
		 * @tparam T: Specific Widget class
		 * @tparam args: Arguments of constructor of given Widget class
		 * @return Instance of widget
		 */
		template<class T, class ...Args>
		T& AddWidget(Args&&... args);

		/**
		 * Remove widget
		 * @param widget: Widget to remove
		 * @warning Don't call it when widget is drawing
		 */
		void RemoveWidget(const Window::AWindow& widget);

		/**
		 * Remove all widgets
		 * @warning Don't call it when widget is drawing
		 */
		void RemoveAllWidgets();

		/**
		 * Open or close all widgets
		 * @param opened: If true, open all widgets, otherwise it will close them all.
		 */
		void OpenAllWidgets(bool opened);

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
		 * Build dockspace
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
		void Draw() override;
	};

	template<class T, class ...Args>
	inline T& Canvas::AddWidget(Args&& ...args)
	{
		static_assert(std::is_base_of<Window::AWindow, T>::value);

		std::unique_ptr<T> widget = std::make_unique<T>(std::forward<Args>(args)...);
		T& output = *widget;
		m_widgets.emplace_back(std::move(widget));

		m_view->Add<Menu::MenuItem>(output.GetName().c_str(), "", &output.isVisible, true).OnClick +=
			[&output]
			{
				if (output.isVisible)
					output.Focus();
			};

		return output;
	}
}
