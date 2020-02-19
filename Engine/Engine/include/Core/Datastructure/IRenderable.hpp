#pragma once

#include <GLFW/glfw3.h>

#include "IComponent.hpp"

namespace Core::Datastructure
{
	class IRenderable : public virtual IComponent
	{
	public:
		GLuint m_program = 0;
		GLuint m_texture = 0;
		GLuint m_VAO = 0;

		virtual void OnDraw() = 0;

		void	Draw()
		{
			if (IsInit() && m_isActive)
				OnDraw();
		}
	};
}