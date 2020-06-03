#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "json.hpp"
using nlohmann::json;

#include "Vec3.hpp"
#include "Color.hpp"
#include "Texture.h"
#include "CoreMinimal.h"
#include "Shader.h"

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

	/**
	 * Contains the value as a rttr variant and the name for send to OpenGl
	 */
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
		Core::SystemManagement::EventSystem<std::string>	UpdateNameMaterial;
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
		bool									IsDelete{ false };
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

		/**
		 * Update material with a new shader
		 * @param newShader: New shader to use
		 */
		void UpdateMaterialShader(std::shared_ptr<Shader> newShader);

		/**
		 * Send to the shader the value of the material
		 */
		void SendMaterial();

		/**
		 * Send to the shader the variants values of the material
		 */
		void SendVariants(GLuint unitTexture);


		/*
		 * Send to the shader of the material a float
		 * @param name: the name of the float in the shader
		 * @param f: the float to send
		 */
		void Sendfloat(const std::string& name, float f)
		{
			if (shader)
			{
				shader->UseProgram();
				glUniform1f(shader->GetLocation(name.c_str()), f);
			}
		}

		/**
		 * Save the values of the material in a file .bmat
		 * @param pathMaterial: the local path of the file
		 * @param shaderPath: the shader path
		 */
		void  SaveMaterial(const std::string& pathMaterial, Resources::Loader::ResourcesManager* shaderPath);

		/**
		 * Load the values of the material in a file .bmat
		 * @param pathMaterial: the local path of the file
		 * @param resources: the resources manager
		 * @retur true if the load succed 
		 */
		bool  LoadMaterialFromJSON(const std::string& pathMaterial, Resources::Loader::ResourcesManager* resources);

		/**
		 * Save all the variants in the .bmat
		 * @param index: the index in the array of variant
		 * @return the json create
		 */
		json SaveVariants(const int& index, Resources::Loader::ResourcesManager* resources);

		/**
		 * Load all the variant in the .bmat
		 * @param index: the index in the array of variant
		 * @param j: the json who stock the value
		 * @param resources: the resources manager
		 */
		void LoadVariants(const int& index, json j, Resources::Loader::ResourcesManager* resources);

		/**
		 * Create a default Material
		 * @param resources: the resources manager
		 */
		void CreateDefaultMaterial(Resources::Loader::ResourcesManager* resources);
	};
}

