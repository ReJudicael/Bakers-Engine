#pragma once

#include "AWindow.h"
#include "EditorCamera.h"
#include "Model.h"
#include "Shader.h"

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

		std::vector<std::shared_ptr<Resources::Model>>		m_model;
		std::shared_ptr<Resources::Shader>					m_shader;

		Core::Maths::Mat<4, 4> m_cube{ m_cube.Identity() };
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

		/**
		 * Display Scene
		 */
		void DisplayScene();

		/**
		 * Check if Gizmos can be used depending on mouse pos
		 */
		void CheckGizmoUse();

		/**
		 * Manipulate cube view gizmo and apply result to camera
		 */
		void GizmoViewManipulateResult();

		/**
		 * Manipulate gizmo and apply result to selected object
		 */
		void GizmoManipulateResult();

		/**
		 * Get direction for raycast from camera forward and mouse position
		 */
		Core::Maths::Vec3 GetCameraDirFromInput();

		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
