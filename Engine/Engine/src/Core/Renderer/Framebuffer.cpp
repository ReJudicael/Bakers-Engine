#include "Framebuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core::Renderer
{
	Framebuffer::Framebuffer(int width, int height) noexcept : FBO{ 0 }, ColorTexture{ 0 }, DepthStencilRenderbuffer{ 0 }, Size(), userPtr{ nullptr }
	{
		GLint PreviousFramebuffer;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);

		// Create Framebuffer that will hold 1 color attachement
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Create texture that will be used as color attachment
		glGenTextures(1, &ColorTexture);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glObjectLabel(GL_TEXTURE, ColorTexture, -1, "ColorTexture");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Note: Here we store the depth stencil in a renderbuffer object, 
		// but we can as well store it in a texture if we want to display it later
		glGenRenderbuffers(1, &DepthStencilRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
		glObjectLabel(GL_RENDERBUFFER, DepthStencilRenderbuffer, -1, "DepthStencilRenderbuffer");
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		// Setup attachements
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, ColorTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRenderbuffer);

		unsigned int DrawAttachments[1] = { GL_COLOR_ATTACHMENT0 + 0 };
		glDrawBuffers(1, DrawAttachments);

		GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "demo_impostor::framebuffer failed to complete (0x%x)\n", FramebufferStatus);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);

		Size[0] = 0;
		Size[1] = 0;
		Size[2] = width;
		Size[3] = height;
	}

	Framebuffer::~Framebuffer() noexcept
	{
		glDeleteTextures(1, &ColorTexture);
		glDeleteRenderbuffers(1, &DepthStencilRenderbuffer);
		glDeleteFramebuffers(1, &FBO);
	}

	void	Framebuffer::Resize(int width, int height) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		Size[2] = width;
		Size[3] = height;
	}
}