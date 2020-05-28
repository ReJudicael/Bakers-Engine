#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "Framebuffer.h"

namespace Core::Renderer
{
	// Class handling post process effects. Only works on objects that also have a camera component
	BAKERS_API_CLASS Postprocess : public Core::Datastructure::ComponentBase
	{

		Framebuffer* m_effectFbo{ nullptr };
		Framebuffer* m_addedFbo{ nullptr }; // Extra fbo for effects that can't be done with a single one

		std::shared_ptr<Resources::Shader> m_blurShader;
		std::shared_ptr<Resources::Shader> m_colorCustomShader;
		std::shared_ptr<Resources::Shader> m_colorBWShader;


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
		 * Draw all active effects in given framebuffer
		 * @param drawFBO: Framebuffer with scene drawn on its color texture
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
