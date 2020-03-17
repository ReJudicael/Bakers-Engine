#include "Texture.h"
#include "TextureData.h"
#include "LoadResources.h"


namespace Resources
{
	void Texture::LoadTexture(const std::string& pathTexture, Resources::Loader::ResourcesManager& resources)
	{
		std::shared_ptr<TextureData> textureData = std::make_shared<TextureData>();

		textureData->nameTexture = pathTexture;
		resources.PushTextureToLink(textureData);
		textureData->textureptr = shared_from_this();
		textureData->CreateTextureFromImage(pathTexture, resources);
	}
}