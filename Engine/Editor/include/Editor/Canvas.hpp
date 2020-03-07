#pragma once

#include "IWidget.h"

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>
#include <vector>
#include <memory>

namespace Editor
{
	/**
	 * Container that holds widgets
	 */
	class Canvas final
	{
	private:
		/**
		 * Widgets contained in canvas
		 */
		std::vector<std::unique_ptr<Widget::IWidget>> m_widgets;

	public:
		/**
		 * Add widget to canvas
		 * @tparam T: Specific  Widget class
		 * @tparam args: Arguments of constructor of given Widget class
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
		 * Display widgets in canvas
		 */
		void Draw();
	};

	template<class T, class ...Args>
	inline void Canvas::AddWidget(Args&&... args)
	{
		static_assert(std::is_base_of<Widget::IWidget, T>::value);
		m_widgets.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
}
