#pragma once
#include <iostream>
#include <stb_image.h>

#include "TextureData.h"
#include "Assimp/texture.h"
#include "Assimp/material.h"
#include "LoadResources.h"
#include "Texture.h"

namespace Resources
{
	void TextureData::CreateTextureFromImage(const char* filename, Loader::ResourcesManager& resources, bool shouldFlip)
	{
		std::string s = filename;

		if (resources.GetCountTextures(filename) > 0)
		{
			textureptr = resources.GetTexture(filename);
			return;
		}

		std::cout << filename << std::endl;
		// load and generate the texture
		int nrChannels;
		stbi_set_flip_vertically_on_load(shouldFlip);
		data = stbi_load(s.c_str(), &width, &height, &nrChannels, 4);

		if (!data)
		{
			std::cout << "Failed to load " << filename << std::endl;
			stbi_image_free(data);
			stateTexture = EOpenGLLinkState::LOADPROBLEM;
			return;
		}
		stateTexture = EOpenGLLinkState::CANLINK;
		resources.EmplaceTextures(filename, textureptr);
	}

	void TextureData::EmplaceInTexture()
	{
		textureptr->stateTexture = stateTexture;
		textureptr->texture = texture;
	}
}