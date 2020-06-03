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
		bool m_showNavMesh{ true };
		bool m_showColliders{ true };

		Datastructure::EditorCamera* m_cam{ nullptr };
		std::shared_ptr<Resources::Shader> m_shader{ nullptr };

		Core::Maths::Mat<4, 4> m_cube{ m_cube.Identity() };
		std::vector<std::shared_ptr<Resources::Model>> m_model;

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
		 * Draw physX collider
		 */
		void DrawAllColliders();

		/**
		 * Draw Guizmo
		 * @param windowSize: Window Size
		 */
		void DrawGuizmo(ImVec2 windowSize);

		/**
		 * Draw Options
		 */
		void DrawOptions();

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
