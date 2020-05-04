#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

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
	struct Texture : public std::enable_shared_from_this<Texture>
	{
		GLuint texture;
		EOpenGLLinkState stateTexture{EOpenGLLinkState::CANTLINK};

		/*
		 * Get the pointer of Texture
		 */
		std::shared_ptr<Texture> getPtr()
		{
			return shared_from_this();
		}

		/*
		 * Load a Texture, and create an TextureData
		 * @param pathTexture: The path of the texture we want to load
		 * @param resources: The resourcesManager
		 */
		void LoadTexture(const std::string& pathTexture, Loader::ResourcesManager& resources);

		~Texture() = default;
	};
}