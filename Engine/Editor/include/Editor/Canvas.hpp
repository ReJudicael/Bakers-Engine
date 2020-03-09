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
	class Canvas final : public IDrawable
	{
	private:
		/**
		 * Widgets contained in canvas
		 */
		std::vector<std::unique_ptr<Widget::IWidget>> m_widgets;

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
