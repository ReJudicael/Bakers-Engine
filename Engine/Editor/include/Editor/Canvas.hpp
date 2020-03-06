#pragma once

#include "IWidget.h"

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>
#include <vector>
#include <memory>

namespace Editor
{
	/**
	 * Contains that holds widgets
	 */
	class Canvas
	{
	public:
		/**
		 * Widgets contained in canvas
		 */
		std::vector<std::unique_ptr<Widget::IWidget>> m_widgets;

	public:
		/**
		 * Add widget in canvas
		 * @tparam T: specific class Widget
		 * @tparam args: arguments of constructor of specific class Widget
		 */
		template<class T, class... Args>
		void AddWidget(Args&&... args);

		/**
		 * Remove widget in canvas
		 */
		// TODO
		void RemoveWidget();

		/**
		 * Remove all widgets
		 */
		void RemoveAllWidgets();

	private:
		/**
		 * Set Dockspace
		 */
		void SetDockspace();

	public:
		/**
		 * Draw contained in canvas
		 */
		void Draw();
	};

	template<class T, class ...Args>
	inline void Canvas::AddWidget(Args&&... args)
	{
		m_widgets.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
}
