#ifndef __TEXT_RENDERER__
#define __TEXT_RENDERER__

#include "CoreMinimal.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

#include "Character.h"
#include "ResourcesManager.h"
#include "IRenderable.hpp"
#include "ComponentBase.h"

namespace Core::Renderer
{
	/**
	 * Handle text displaying in scene
	 */
	class TextRenderer : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
	{
	protected:
		std::string	m_text{ "Add text here" };
		std::string	m_font;
		int			m_size{ 12 };

		std::shared_ptr<Resources::Shader>	m_shader;
		std::shared_ptr<Resources::Model>	m_quad;
	public:
		/**
		 * Default constructor
		 */
		TextRenderer();
		/**
		 * Destructor
		 */
		~TextRenderer() = default;

		virtual void	OnInit() override;

		virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr) override;
		virtual void OnReset() override;

		/**
		 * Text setter
		 * @return Text
		 */
		const std::string&	GetText() const
		{
			return m_text;
		}
		/**
		 * Text getter
		 * @param text: New text
		 */
		void				SetText(const std::string& text)
		{
			m_text = text;
		}

		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	StartCopy(IComponent*& copyTo) const override;

		REGISTER_CLASS(ComponentBase, IRenderable);
	};
}

#endif
