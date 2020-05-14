#pragma once

#include "CoreMinimal.h"
#include "Shader.h"

namespace Core::Renderer
{
	enum class FBOType
	{
		CAMERA, //Rendering for cameras
		POSTPROCESSING, //Rendering 
		CUSTOM //Custom fbo type for rendering of other types that those described
	};

	struct FramebufferData
	{
		std::shared_ptr<Resources::Shader> s = nullptr;
		GLuint VAO;
		bool active = true;
	};

	BAKERS_API_STRUCT Framebuffer
	{
		FramebufferData data;
		GLuint	FBO;
		GLuint	ColorTexture;
		GLuint	DepthStencilRenderbuffer;
		int		Size[4];
		void*	userPtr;
		FBOType	type;

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer(int width = 1280, int height = 800, FBOType t = FBOType::CUSTOM) noexcept;
		~Framebuffer() noexcept;

		void	InitPostProcess(std::shared_ptr<Resources::Shader> s);

		void	Resize(int width, int height) noexcept;
	};
}