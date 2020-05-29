#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Color.hpp"

namespace Core::Renderer
{
	// Class handling post process effects. Only works on objects that also have a camera component
	BAKERS_API_CLASS Postprocess : public Core::Datastructure::ComponentBase
	{
		bool	m_useBlur{ false };
		bool	m_useColorCustom{ false };
		bool	m_useBW{ false };
		bool	m_useBloom{ false };

		GLuint	m_VAO;

		Maths::Color m_correctedColor;

		Framebuffer* m_effectFbo{ nullptr };
		Framebuffer* m_addedFbo{ nullptr }; // Extra fbo for effects that can't be done with a single one

		std::shared_ptr<Resources::Shader> m_defaultShader;
		std::shared_ptr<Resources::Shader> m_blurShader;
		std::shared_ptr<Resources::Shader> m_colorCustomShader;
		std::shared_ptr<Resources::Shader> m_colorBWShader;	
		std::shared_ptr<Resources::Shader> m_additionShader;	


		protected:
			/**
			 * Copies the data of the component into the given component.
			 * Should always be safe to cast pointer to current component type.
			 */
			virtual void	OnCopy(IComponent * copyTo) const override;
			/**
			 * Copies the component in the given pointer.
			 * On override, should not call other versions of the function.
			 */
			virtual void	StartCopy(IComponent*& copyTo) const override;

			/**
			 * First frame upon creation event
			 */
			virtual bool	OnStart() override;

			/**
			 * Destroy event
			 */
			virtual void	OnDestroy() override;

			/**
			 * Reset event
			 */
			virtual void	OnReset() override;

			/**
			 * Effect check for variable display in editor
			 * @return True if blur effect is checked
			 */
			bool	IsUsingBlur() { return CanUseEffects() && m_useBlur; }

			/**
			 * Effect check for variable display in editor
			 * @return True if color customization effect is checked
			 */
			bool	IsUsingColor() { return CanUseEffects() && m_useColorCustom; }

			/**
			 * Effect check for variable display in editor
			 * @return True if bloom is not activated (when bloom is used, other effects cannot be activated)
			 */
			bool	CanUseEffects() { return !m_useBloom; }

	public:
		/**
		 * Default Constructor
		 */
		Postprocess();

		/**
		 * Destructor
		 */
		~Postprocess();

		/**
		 * Init event
		 */
		void	OnInit() override;

		/**
		 * Init VAO with which postprocess will be drawn
		 */
		void	InitPostprocessVAO();

		/**
		 * Draw single effect on framebuffer
		 * @param from: Framebuffer from which the color texture is used
		 * @param to: Framebuffer binded to draw color texture after effect
		 * @param effect: Shader with postprocess effect to draw
		 */
		void	DrawEffect(Framebuffer* from, Framebuffer* to, std::shared_ptr<Resources::Shader> effect);

		/**
		 * Draw for specific behavior of bloom effect
		 * @param drawFbo: Framebuffer with scene drawn on its color texture
		 */
		void	DrawBloom(Framebuffer* drawFbo);

		/**
		 * Draw all active effects in given framebuffer
		 * @param drawFbo: Framebuffer with scene drawn on its color texture
		 */
		void	DrawPostProcess(Framebuffer* drawFbo);

		/**
		 * Resize framebuffers
		 * @param width: New width 
		 * @param height: New height
		 */
		void	Resize(unsigned width, unsigned height);

		REGISTER_CLASS(ComponentBase)
	};

}
