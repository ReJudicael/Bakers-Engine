#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "IComponent.hpp"
#include "ICamera.h"
#include <list>
#include "Debug.h"
#include "Shader.h"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	/**
	 * Component interface to make a component that is drawn.
	 */
	BAKERS_API_CLASS IRenderable : public virtual IComponent
	{
	protected:
		/**
		 * Called for rendering. Must be overriden if the component is inherited
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param givenShader: Shader to use instead of material shader if provided
		 */
		virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr) = 0;
		virtual void OnCopy(IComponent* copyTo) const override;
		virtual void OnReset() override;
	public:
		/**
		 * Destructor of the renderable interface
		 */
		~IRenderable() noexcept;
		virtual void	OnInit() override;

		GLuint m_texture = 0;
		GLuint m_VAO = 0;

		/**
		 * Draw renderable
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param givenShader: Shader to use instead of material shader if provided
		 */
		void Draw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr);

		REGISTER_CLASS(IComponent)
	};
}