#pragma once

#include "IRenderable.h"

#include <GLFW/glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

namespace Core
{
	/*
	 * UI Theme
	 */
	enum class GUIStyle : short int
	{
		CLASSIC = 0,
		DARK,
		LIGHT,
		BAKER
	};

	/*
	 * Create, Draw and Destroy UI
	 */
	class GUIManager
	{
	public:
		/*
		 * Set and init ImGUI
		 * @param glfwWindow: GLFWwindow to setup Platform/Renderer bindings
		 * @param glsl_version: Store GLSL version string so we can refer to it later in case we recreate shaders
		 * @param style: UI Theme
		 */
		GUIManager(GLFWwindow* glfwWindow, const char* glsl_version, GUIStyle style);

		/**
		 * Clean up ImGui
		 */
		~GUIManager();

	private:
		/*
		 * Set IO Configuration
		 */
		void SetIOConfiguration();

	public:
		/*
		 * Set UI Theme
		 * @param style: UI Theme wanted
		 * @return Returns true if the theme of the user interface is correctly defined, otherwise false
		 */
		bool SetStyle(GUIStyle style);

		/*
		 * Draw UI
		 */
		void Render();
	};
}
