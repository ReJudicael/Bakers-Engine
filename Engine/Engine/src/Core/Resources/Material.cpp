#include "Material.h"
#include "Assimp/scene.h"
#include "Assimp/material.h"
#include "Assimp/texture.h"
#include "LoadResources.h"
#include "Texture.h"
#include "TextureData.h"


namespace Resources
{
	void Material::LoadMaterialFromaiMaterial(aiMaterial* mat, const std::string& directory, 
												Loader::ResourcesManager& resources)
	{

		textures.resize(2);

		shader = resources.GetShader("Default");
		LoadTextureMaterial(mat, textures[0], aiTextureType_DIFFUSE, directory, resources);
		LoadTextureMaterial(mat, textures[textures.size()- 1], aiTextureType_NORMALS, directory, resources);
		// maybe load a normalMap

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		specularColor = { color.r, color.g, color.b };
		materialColor = { 1.0f, 1.0f, 1.0f };
	}

	void Material::LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture,
					const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager& resources)
	{
		if (mat->GetTextureCount(textureType) > 0)
		{
			aiString path;
			if (mat->GetTexture(textureType, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				resources.LoadTexture(fullPath, texture);
				if (textureType == aiTextureType_NORMALS)
					shader = resources.GetShader("NormalMapDefault");
			}
		}
		else
		{
			textures.resize(textures.size() - 1);
		}

	}

	void Material::SendMaterial()
	{
		glUniform3fv(shader->GetLocation("mat.diffuseColor"), 1, diffuseColor.xyz);
		glUniform3fv(shader->GetLocation("mat.ambientColor"), 1, ambientColor.xyz);
		glUniform3fv(shader->GetLocation("mat.specularColor"), 1, specularColor.xyz);
	}
}