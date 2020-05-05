#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
//#include <memory>

#include "OpenGLLinkState.h"
#include "CoreMinimal.h"


struct aiMaterial;
enum aiTextureType : int;

namespace Resources
{
	struct Texture;
	namespace Loader
	{
		class ResourcesManager;
	}

	/**
	 * Class use for create a Texture,
	 * contains all the values use for link a Texture to OpenGL
	 */
	struct TextureData
	{
		GLuint ID{0};
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
		std::string nameTexture;
		std::shared_ptr<Texture> textureptr;

		/**
		 * Create a texture from an image (.png, .jpg, etc...)
		 * @param pathTexture: The path of the texture we want to load
		 * @param resources: The resourcesManager
		 * @param shouldFlip: Default value = true.
		 * if true flip the texture
		 */
		void CreateTextureFromImage(const std::string& pathTexture, Loader::ResourcesManager& resources, bool shouldFlip = true);

		/**
		 * Link the Texture to OpenGL
		 */
		void CreateOpenGLTexture();

		/**
		 * Emplace all the usefull value in the Texture
		 */
		void EmplaceInTexture();
	};
}