#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "OpenGLLinkState.h"

namespace Resources
{
	struct Texture
	{
		GLuint texture;
		EOpenGLLinkState stateTexture;
	};
}