#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>

#include "Vec3.hpp"
#include "Texture.h"
#include "Debug.h"

struct aiMaterial;
struct aiScene;
enum aiTextureType;

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}
	struct Texture;
	struct Material : public std::enable_shared_from_this<Material>
	{
		Core::Maths::Vec3 diffuseColor;
		Core::Maths::Vec3 ambientColor;
		Core::Maths::Vec3 specularColor;

		Core::Maths::Vec3 materialColor;

		std::shared_ptr<GLuint> program;

		std::vector<std::shared_ptr<Texture>> textures;

		void LoadMaterialFromaiMaterial(aiMaterial* mat, const std::string& directory, 
										Loader::ResourcesManager& resources);
		void LoadaiSceneMaterial(const aiScene* scene, const unsigned int& mMaterialIndex, const std::string& directory,
								Loader::ResourcesManager& resources, const int meshNameCall = 0);

		void LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& textureData, 
									const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager& resources);
	};
}

