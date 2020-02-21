#pragma once

#include <GLFW/glfw3.h>

#include "IComponent.hpp"

namespace Core::Datastructure
{
	/**
	 * Component interface to make a component that is drawn.
	 */
	class IRenderable : public virtual IComponent
	{
	public:
		virtual void	OnStart() override;

		GLuint m_program = 0;
		GLuint m_texture = 0;
		GLuint m_VAO = 0;

		/**
		 * Called for rendering. Must be overriden if the component is inherited
		 */
		virtual void OnDraw() = 0;

		/**
		 * Function called by the engine to draw the object
		 */
		void	Draw()
		{
			if (IsInit() && m_isActive)
				OnDraw();
		}
	};
}