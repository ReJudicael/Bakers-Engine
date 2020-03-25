#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

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

	struct TextureData
	{
		GLuint ID{0};
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
		std::string nameTexture;
		std::shared_ptr<Texture> textureptr;

		// just call by the material during a load mesh
		void CreateTextureFromImage(const std::string& filename, Loader::ResourcesManager& resources, bool shouldFlip = true);

		void CreateOpenGLTexture();

		void EmplaceInTexture();
	};
}