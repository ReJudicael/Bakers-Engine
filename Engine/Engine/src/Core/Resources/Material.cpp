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
		Loader::ResourcesManager* resources, std::shared_ptr<Resources::Loader::ImporterData>& importer)
	{
		importer->maxUseOfImporter++;
		textures.resize(2);

		shader = resources->GetShader("Default");
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
		mat->Get(AI_MATKEY_SHININESS, shininess);
		mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
		materialColor = { 1.0f, 1.0f, 1.0f };
		shaderChoose = true;

		importer->maxUseOfImporter--;
	}

	void Material::LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture,
					const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager* resources)
	{
		if (mat->GetTextureCount(textureType) > 0)
		{
			aiString path;
			if (mat->GetTexture(textureType, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				//texture = std::make_shared<Texture>();
				resources->LoadTexture(fullPath, texture);
				if (textureType == aiTextureType_NORMALS)
					shader = resources->GetShader("NormalMapDefault");
			}
		}
		else
		{
			textures.resize(textures.size() - 1);
		}

	}

	void Material::SendMaterial()
	{
		glUniform3fv(shader->GetLocation("umat.ambientColor"), 1, ambientColor.rgb);
		glUniform3fv(shader->GetLocation("umat.diffuseColor"), 1, diffuseColor.rgb);
		glUniform3fv(shader->GetLocation("umat.specularColor"), 1, specularColor.rgb);
		glUniform1f(shader->GetLocation("umat.shininess"), shininess);
		glUniform1f(shader->GetLocation("umat.shininessStrength"), shininessStrength);

		GLuint unit{ 0 };
		// send all the texture
		for (auto i{ 0 }; i < textures.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (textures[i])
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textures[0]->texture);
					unit++;
				}
				break;
			case 1:
				if (textures[i])
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, textures[1]->texture);
					unit++;
				}
				break;
			}
		}

		for (auto i{ 0 }; i < variants.size(); i++)
		{
			if (variants[i].var.can_convert<float>())
			{
				float tmp;
				if(variants[i].var.convert<float>(tmp))
					glUniform1f(shader->GetLocation(variants[i].name.c_str()), tmp);
					
			}
			else if (variants[i].var.can_convert<std::shared_ptr<Texture>>())
			{
				std::shared_ptr<Texture> text;
				variants[i].var.convert<std::shared_ptr<Texture>>(text);
				if (text)
				{
					unit++;
					glActiveTexture(GL_TEXTURE0 + unit);
					glBindTexture(GL_TEXTURE_2D, text->texture);
					glUniform1i(shader->GetLocation(variants[i].name.c_str()), unit);
				}
			}
		}
	}

	void Material::UpdateMaterialShader(std::shared_ptr<Shader> newShader)
	{
		shader = newShader;

		variants.clear();
		InitVariantUniform();
	}

	void Material::InitVariantUniform()
	{
		GLint count;
		GLsizei length;
		GLchar name[50];
		GLint size;
		GLenum type;

		int numberOfBasicTexture{ 0 };

		glGetProgramiv(shader->GetProgram(), GL_ACTIVE_UNIFORMS, &count);
		for (auto i{ 0 }; i < count; i++)
		{
			glGetActiveUniform(shader->GetProgram(), (GLuint)i, 50, &length, &size, &type, name);

			if (name[0] != 'u')
			{
				VariantUniform uni;
				uni.name = name;
				rttr::variant var;
				switch (type)
				{
				case GL_FLOAT:
					uni.var = 0.f;
					variants.push_back(uni);
					break;
				case GL_INT:
					uni.var = 0;
					variants.push_back(uni);
					break;
				case GL_UNSIGNED_INT:
					uni.var = static_cast<unsigned int>(0);
					variants.push_back(uni);
					break;
				case GL_SAMPLER_2D:
					uni.var = std::shared_ptr<Texture>();
					variants.push_back(uni);
					break;
				}
			}
			else if (type == GL_SAMPLER_2D)
				numberOfBasicTexture++;
		}

		textures.resize(numberOfBasicTexture);
	}
}