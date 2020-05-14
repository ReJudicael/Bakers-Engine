#include "Framebuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Renderer;
	rttr::registration::enumeration<FBOType>("FBO types")
		(
			value("Camera", FBOType::CAMERA),
			value("PostProcess", FBOType::POSTPROCESSING),
			value("Custom", FBOType::CUSTOM)
		);
}

namespace Core::Renderer
{
	Framebuffer::Framebuffer(int width, int height, FBOType t) noexcept : FBO{ 0 }, ColorTexture{ 0 }, DepthStencilRenderbuffer{ 0 }, Size(), userPtr{ nullptr }, type { t }
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

	void	Framebuffer::InitPostProcess(std::shared_ptr<Resources::Shader> s)
	{
		GLuint VBO;

		data.s = s;
		// Gen unit quad
		{
			float Quad[24] =
			{
				 -1.f,-1.f ,  0.f, 0.f, // bl
				  1.f,-1.f ,  1.f, 0.f, // br
				  1.f, 1.f,  1.f, 1.f, // tr

				-1.f, 1.f ,  0.f, 1.f , // tl
				-1.f,-1.f,   0.f, 0.f, // bl
				 1.f, 1.f ,  1.f, 1.f, // tr
			};

			// Upload mesh to gpu
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), Quad, GL_STATIC_DRAW);
		}

		// Create a vertex array and bind it
		glGenVertexArrays(1, &data.VAO);
		glBindVertexArray(data.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
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