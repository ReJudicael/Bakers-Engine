#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
		GLuint texture{0};
		unsigned char* data;
		int height;
		int width;
		EOpenGLLinkState stateTexture;
		std::string nameTexture;
		std::shared_ptr<Texture> textureptr;

		// just call by the material during a load mesh
		void CreateTextureFromImage(const char* filename, Loader::ResourcesManager& resources, bool shouldFlip = true);

		// TO DO 
		// do the CreateTextureFromImage but without call of the material

		void EmplaceInTexture();
	};
}