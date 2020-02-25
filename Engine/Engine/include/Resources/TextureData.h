#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "OpenGLLinkState.h"


namespace Resources
{
	struct TextureData
	{
		std::shared_ptr<GLuint> texture;
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
	};
}