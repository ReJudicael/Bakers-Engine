#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "Vec3.hpp"
#include "Color.hpp"
#include "Texture.h"
#include "CoreMinimal.h"

struct aiMaterial;
struct aiScene;
enum aiTextureType : int;

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
		struct ImporterData;
	}
	struct Texture;
	class Shader;

	struct VariantUniform
	{
		std::string name;
		rttr::variant var;
	};

	/**
	 * Contains the values with which we can draw the color of the mesh
	 */
	BAKERS_API_STRUCT Material
	{
		Core::Maths::Color						diffuseColor{};
		Core::Maths::Color						ambientColor{};
		Core::Maths::Color						specularColor{};
		float									shininess{};
		float									shininessStrength{};

		Core::Maths::Vec3						materialColor{};

		std::vector<std::shared_ptr<Texture>>	textures{};
		std::vector<VariantUniform>				variants{};
		bool									shaderChoose{ false };
		bool									IsSkeletal{ false };
		std::shared_ptr<Shader>					shader{};

		Material() = default;

		/**
		 * Load the current material with an aiMaterial from assimp
		 * @param mat: the aiMaterial with which we want to load
		 * @param directory: the folder path of the 3D Object
		 * @param resources: the ResouresManager
		 */
		void LoadMaterialFromaiMaterial(aiMaterial* mat, const std::string& directory, 
										Loader::ResourcesManager* resources,
										std::shared_ptr<Resources::Loader::ImporterData>& importer);

		/**
		 * Ask for load a texture from an aiMaterial
		 * @param mat: the aiMaterial with which we want to load
		 * @param texture: the texture that we want to change
		 * @param textureType: the type of the texture for assimp
		 * @param directory: the folder path of the 3D Object
		 * @param resources: the ResouresManager
		 */
		void LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture, 
									const aiTextureType& textureType, const std::string& directory, Loader::ResourcesManager* resources);
		/*
		 * Create all the variable compared to 
		 * the uniforms variable who haven't an 'u'
		 * in the beginning
		 */
		void InitVariantUniform();

		void UpdateMaterialShader(std::shared_ptr<Shader> newShader);

		/**
		 * Send to the shader the value of the material
		 */
		void SendMaterial();

		/**
		 * Send to the shader the variants values of the material
		 */
		void SendVariants(GLuint unitTexture);

		void CreateDefaultMaterial(Resources::Loader::ResourcesManager* resources);
	};
}

