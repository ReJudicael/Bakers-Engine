#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "OpenGLLinkState.h"


struct aiMaterial;
enum aiTextureType;

namespace Resources
{
	struct Texture;
	namespace Loader
	{
		class ResourcesManager;
	}

	struct TextureData
	{
		GLuint texture{0};
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
		std::string nameTexture;
		std::shared_ptr<Texture> textureptr;

		void CreateTextureFromImage(const char* filename, Loader::ResourcesManager& resources, bool shouldFlip = true);

		void EmplaceInTexture();
	};
}