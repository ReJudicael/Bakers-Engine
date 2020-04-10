#pragma once

#include "Canvas.hpp"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

/**
 * Contains the editor of the engine
 */
namespace Editor
{
	class EditorEngine;
	/**
	 * UI Color scheme and appearence
	 */
	enum class GUIStyle : unsigned short int
	{
		CLASSIC = 0,
		DARK,
		LIGHT,
		BAKER
	};

	/**
	 * Manager to handle UI creation, displaying and destruction
	 */
	class GUIManager final
	{
	private:
		/**
		 * Contains elements to display (Widgets) and set dockspace
		 */
		Canvas* m_canvas{ nullptr };

		/**
		 * Pointer to engine core
		 */
		EditorEngine* m_engine;

	public:
		/**
		 * Set and init ImGUI
		 * @param engine: EditorEngine to setup Platform/Renderer bindings
		 * @param glsl_version: Store GLSL version string so we can refer to it later in case we recreate shaders
		 * @param style: UI Theme
		 */
		GUIManager(EditorEngine* engine, const char* glsl_version, GUIStyle style);

		/**
		 * Clean up canvas and ImGui
		 */
		~GUIManager();

	private:
		/**
		 * Set IO Configuration
		 */
		void SetIOConfiguration();

	private:
		/**
		 * Set Baker UI Theme
		 */
		void SetStyleBaker();

	public:
		/**
		 * Set UI Theme
		 * @param style: Desired UI Theme
		 * @return Returns true if the theme of the user interface is correctly defined, otherwise false
		 */
		bool SetStyle(GUIStyle style);

		/**
		 * Set Canvas to GUIManager
		 * @param canvas: Desired canvas
		 */
		void SetCanvas(Canvas* canvas);

	private:
		/**
		 * Update ImGui
		 */
		void NewFrame();

		/**
		 * Render ImGui
		 */
		void EndFrame();

	public:
		/**
		 * Draw UI
		 */
		void Render();

		/**
		 * Indicate whether the given window is focused or not
		 * @param id: The id of the window within the canvas vector
		 * @return True if the given window is focused or false if it is not or the id is invalid
		 */
		bool IsWindowFocused(const size_t id);

	public:
		/**
		 * Returns pointer to engine core
		 */
		EditorEngine* GetEngine() noexcept;
	};
}
