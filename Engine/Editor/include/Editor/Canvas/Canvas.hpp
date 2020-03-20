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
	class EditorEngine;
	class GUIManager;
	/**
	 * Dockspace handler
	 */
	class Canvas final : public virtual Datastructure::IDrawable, public Datastructure::Container<Window::AWindow>
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
		 * Menu bar to display menu groups and menu items
		 */
		MenuBar* m_menuBar;

		/**
		 * Menu group "View" to display or not a widget 
		 */
		Widget::MenuGroup* m_view;

		/**
		 * Parent of the canvas
		 */
		GUIManager* m_manager;
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
		 * Initialize Menur Bar
		 */
		void InitMenuBar();

		/**
		 * Returns the manager of the canvas
		 */
		GUIManager* GetManager() noexcept;
		
		/**
		 * Sets the manager of the canvas
		 * @param manager: The manager of the canvas
		 */
		void		SetManager(GUIManager* manager) noexcept;

		/**
		 * Returns a pointer to the engine core
		 */
		EditorEngine* GetEngine() noexcept;

		/**
		 * Add widget
		 * @tparam T: Specific Widget class
		 * @tparam args: Arguments of constructor of given Widget class
		 * @return Instance of widget
		 */
		template<class T, class ...Args>
		T& Add(Args&&... args);

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
	inline T& Canvas::Add(Args&& ...args)
	{
		T& output = Datastructure::Container<Window::AWindow>::Add<T, Canvas*, Args...>(this, std::forward<Args>(args)...);

		m_view->Add<Widget::MenuItem>(output.GetName().c_str(), "", &output.isVisible, true).OnClick +=
			[&output]
			{
				if (output.isVisible)
					output.Focus();
			};

		return output;
	}
}
