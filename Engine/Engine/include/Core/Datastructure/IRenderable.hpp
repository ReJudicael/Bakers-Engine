#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "IComponent.hpp"
#include "ICamera.h"
#include <list>
#include "Debug.h"
#include "Shader.h"

namespace Core::Datastructure
{
	/**
	 * Component interface to make a component that is drawn.
	 */
	class IRenderable : public virtual IComponent
	{
	public:
		virtual void	OnStart() override;

		std::shared_ptr<Resources::Shader> m_shader;
		GLuint m_texture = 0;
		GLuint m_VAO = 0;

		/**
		 * Called for rendering. Must be overriden if the component is inherited
		 * @param cam: Camera to render to
		 */
		virtual void OnDraw(ICamera* cam) = 0;

		/**
		 * Function called by the engine to draw the object
		 * @param cameras: Cameras of the scene
		 */
		void	Draw(const std::list<ICamera*>& cameras);
	};
}