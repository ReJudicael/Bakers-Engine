#pragma once

#include "IWidget.h"

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>

#include <vector>
#include <memory>

namespace Editor
{
	/**
	 * Container that holds widgets, aka widget manager
	 */
	class Canvas final : public IDrawable
	{
	private:
		/**
		 * Widgets contained in canvas
		 */
		std::vector<std::unique_ptr<Widget::IWidget>> m_widgets;

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

		/**
		 * Add widget to canvas
		 * @tparam T: Specific Widget class
		 * @tparam args: Arguments of constructor of given Widget class
		 * @return NameID of widget added
		 */
		template<class T, class ...Args>
		std::string AddWidget(Args&&... args);

		/**
		 * Remove widget in canvas
		 * @param name: NameID of widget to remove
		 * @warning Don't call it when widget is drawing
		 */
		void RemoveWidget(std::string nameID);

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
	inline std::string Canvas::AddWidget(Args&&... args)
	{
		static_assert(std::is_base_of<Widget::IWidget, T>::value);

		std::unique_ptr<Widget::IWidget> widget = std::make_unique<T>(std::forward<Args>(args)...);
		const std::string nameWidget = widget->GetNameID();
		m_widgets.emplace_back(std::move(widget));
		return nameWidget;
	}
}
