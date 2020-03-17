#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "OpenGLLinkState.h"

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}

	struct Texture : public std::enable_shared_from_this<Texture>
	{
		GLuint texture;
		EOpenGLLinkState stateTexture{EOpenGLLinkState::CANTLINK};

		std::shared_ptr<Texture> getPtr()
		{
			return shared_from_this();
		}

		void LoadTexture(const std::string& pathTexture, Loader::ResourcesManager& resources);

		~Texture()
		{
			//std::cout << "je me detruit" << std::endl;
		}
	};
}