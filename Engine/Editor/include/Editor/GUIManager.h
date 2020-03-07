#pragma once

#include "Canvas.hpp"

#include <GLFW/glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

/**
 * Contains the editor of the engine
 */
namespace Editor
{
	/**
	 * UI Color scheme and appearence
	 */
	enum class GUIStyle : short int
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
	public:
		/**
		 * Contains elements to display (Widgets) and set dockspace
		 */
		Canvas* m_canvas{ nullptr };

	public:
		/**
		 * Set and init ImGUI
		 * @param glfwWindow: GLFWwindow to setup Platform/Renderer bindings
		 * @param glsl_version: Store GLSL version string so we can refer to it later in case we recreate shaders
		 * @param style: UI Theme
		 */
		GUIManager(GLFWwindow* glfwWindow, const char* glsl_version, GUIStyle style);

		/**
		 * Clean up canvas and ImGui
		 */
		~GUIManager();

	private:
		/**
		 * Set IO Configuration
		 */
		void SetIOConfiguration();

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
	};
}
