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
	void TextureData::CreateTextureFromImage(const std::string& filename, Loader::ResourcesManager& resources, bool shouldFlip)
	{
		// load and generate the texture
		int nrChannels;
		stbi_set_flip_vertically_on_load(shouldFlip);
		data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);

		stateTexture = EOpenGLLinkState::CANLINK;
		if (!data)
		{
			std::cout << "Failed to load " << filename << std::endl;
			stbi_image_free(data);
			stateTexture = EOpenGLLinkState::LOADPROBLEM;
			textureptr->stateTexture = EOpenGLLinkState::LOADPROBLEM;
		}
		resources.EmplaceTextures(filename, textureptr->getPtr());
	}

	void TextureData::CreateOpenGLTexture()
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		data = nullptr;
		glGenerateMipmap(GL_TEXTURE_2D);

		stateTexture = EOpenGLLinkState::ISLINK;
		ID = texture;

		EmplaceInTexture();
	}

	void TextureData::EmplaceInTexture()
	{
		textureptr->stateTexture = stateTexture;
		textureptr->texture = ID;
	}
}