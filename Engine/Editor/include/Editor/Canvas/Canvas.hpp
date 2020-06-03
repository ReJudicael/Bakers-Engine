#pragma once

#include <vector>
#include <memory>

#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>

#include "IDrawable.h"
#include "AWindow.h"

#include "MenuGroup.h"
#include "MenuBar.h"

namespace Resources
{
	struct Texture;
}
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
		 * Menu group "View" to display or not a window 
		 */
		Widget::MenuGroup* m_view;

	private:
		/**
		 * GUIManager of the canvas
		 */
		GUIManager* m_manager;

	public:
		/**
		 * Value constructor
		 * @param manager: GUIManager
		 */
		Canvas(Editor::GUIManager* manager);

		/**
		 * Default destructor
		 */
		~Canvas();

		/**
		 * Initialize Menur Bar
		 */
		void InitMenuBar();

		/**
		 * Add window
		 * @tparam T: Specific Window class
		 * @tparam args: Arguments of constructor of given Window class
		 * @return Instance of window
		 */
		template<class T, class ...Args>
		T& Add(Args&&... args);

		/**
		 * Open or close all windows
		 * @param opened: If true, open all windows, otherwise it will close them all.
		 */
		void SetAllWindowVisibility(bool opened);

		/**
		 * Indicate whether the given window is focused or not
		 * @param id: The id of the window within the canvas vector
		 * @return True if the given window is focused or false if it is not or the id is invalid
		 */
		bool IsWindowFocused(const size_t id) const;

		/**
		 * Get ID of last focused window
		 * @return ID within the canvas vector of the last focused Window
		 */
		size_t GetFocusedWindow() const;

	private:
		/**
		 * Check shortcuts
		 */
		void CheckShortcuts();

	private:
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
		 * Display docked windows in canvas
		 */
		void Draw() override;

	public:
		/**
		 * Returns the manager of the canvas
		 * @return The manager of the canvas
		 */
		GUIManager* GetManager() noexcept;

		/**
		 * Returns the engine core
		 * @return The engine core
		 */
		EditorEngine* GetEngine() noexcept;

#pragma region ToolbarRegion
	private:
		/**
		 * Draw toolbar
		 */
		void DrawToolbar();

		/**
		 * Add a button for toolbar
		 * @param label: Label of button
		 * @param state: Condition (Whether the button should be active or inactive)
		 * @param helpMarker: Text written when the mouse is over the button
		 */
		bool ToolbarButton(const char* label, bool state, const char* helpMarker = (const char*)0);
#pragma endregion
	};

	template<class T, class ...Args>
	inline T& Canvas::Add(Args&& ...args)
	{
		ZoneScoped
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
