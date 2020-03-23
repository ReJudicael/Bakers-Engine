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
	struct Shader;

	/**
	 * Contains the values with which we can draw the color of the mesh
	 */
	struct Material : public std::enable_shared_from_this<Material>
	{
		Core::Maths::Vec3						diffuseColor;
		Core::Maths::Vec3						ambientColor;
		Core::Maths::Vec3						specularColor;
		float									shininess;
		float									shininessStrength;

		Core::Maths::Vec3						materialColor;

		std::vector<std::shared_ptr<Texture>>	textures;

		std::shared_ptr<Shader>					shader;

		/**
		 * Load the current material with an aiMaterial from assimp
		 * @param mat: the aiMaterial with which we want to load
		 * @param directory: the folder path of the 3D Object
		 * @param resources: the ResouresManager
		 */
		void LoadMaterialFromaiMaterial(aiMaterial* mat, const std::string& directory, 
										Loader::ResourcesManager& resources);

		/**
		 * Ask for load a texture from an aiMaterial
		 * @param mat: the aiMaterial with which we want to load
		 * @param texture: the texture that we want to change
		 * @param textureType: the type of the texture for assimp
		 * @param directory: the folder path of the 3D Object
		 * @param resources: the ResouresManager
		 */
		void LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture, 
									const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager& resources);

		void SendMaterial();
	};
}

