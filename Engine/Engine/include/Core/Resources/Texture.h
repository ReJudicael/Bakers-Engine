#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
//#include <memory>

#include "OpenGLLinkState.h"
#include "CoreMinimal.h"

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}

	/*
	 * Contains the ID of the texture, inherit of enable_shared_from_this
	 * for have the function shared_from_this()
	 */
	struct Texture
	{
		GLuint texture;
	};
}