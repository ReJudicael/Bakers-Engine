#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "OpenGLLinkState.h"
#include "Debug.h"


namespace Resources
{
	struct TextureData
	{
		GLuint texture{0};
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
	};
}