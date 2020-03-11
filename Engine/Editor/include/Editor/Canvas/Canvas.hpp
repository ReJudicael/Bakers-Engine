#pragma once

#include "IDrawable.h"
#include "AWidget.h"

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
		 * Widgets list
		 */
		std::vector<std::unique_ptr<Widget::AWidget>> m_widgets;

		/**
		 * Dockspace window flags
		 */
		ImGuiWindowFlags m_dockWindowFlags;

		/**
		 * Dockspace node flags
		 */
		ImGuiDockNodeFlags m_dockNodeFlags;

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
		void RemoveWidget(const Widget::AWidget& widget);

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
		 * Build dockspace
		 */
		void BuildDockspace();

	private:
		/**
		 * Set dockspace
		 */
		void SetDockspace();

		/**
		 * Set Menu bar
		 */
		 // TODO: Class Menu Bar
		void SetMenuBar();

	public:
		/**
		 * Display docked widgets in canvas
		 */
		void Draw() override;
	};

	template<class T, class ...Args>
	inline T& Canvas::AddWidget(Args&& ...args)
	{
		static_assert(std::is_base_of<Widget::AWidget, T>::value);

		std::unique_ptr<T> widget = std::make_unique<T>(std::forward<Args>(args)...);
		T& output = *widget;
		m_widgets.emplace_back(std::move(widget));
		return output;
	}
}
