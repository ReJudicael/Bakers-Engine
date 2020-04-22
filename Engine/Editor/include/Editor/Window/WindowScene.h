#pragma once

#include "AWindow.h"
#include "EditorCamera.h"

namespace Core::Renderer
{
	struct Framebuffer;
}

namespace Editor::Window
{
	/**
	 * Window Scene
	 */
	class WindowScene final : public AWindow
	{
		Datastructure::EditorCamera* m_cam;
	public:
		/**
		 * Constructor which set title of window ("Scene")
		 */
		WindowScene(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowScene() = default;

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;

	private:
		/**
		 * Display Scene
		 */
		void DisplayScene();

		/**
		 * Manipulate gizmo and apply result to selected object
		 * @param matrix: Matrix for gizmo transform
		 */
		void GizmoManipulateResult(float* matrix);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
