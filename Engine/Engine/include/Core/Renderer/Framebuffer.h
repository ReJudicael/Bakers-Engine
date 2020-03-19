#pragma once

#include "CoreMinimal.h"

namespace Core::Renderer
{
	BAKERS_API_STRUCT Framebuffer
	{
		GLuint FBO;
		GLuint ColorTexture;
		GLuint DepthStencilRenderbuffer;
		int	   Size[4];

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer(int width = 1280, int height = 800) noexcept;
		~Framebuffer() noexcept;

		void	Resize(int width, int height) noexcept;
	};
}