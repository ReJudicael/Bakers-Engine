#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include "Assimp/Importer.hpp"

#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "ModelData.h"
#include "TextureData.h"
//#include "Material.h"
#include "Texture.h"
#include "Object3DGraph.h"
#include "Vertex.h" 
#include "Debug.h"
#include "Shader.h"
#include "TaskSystem.hpp"
#include "CoreMinimal.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType : int;
class Core::Datastructure::Object;

/*namespace Assimp
{
	class Importer;
}*/


namespace Resources
{
	struct Model;
	//struct Texture;
	//struct TextureData;
	struct Material;

	using unorderedmapTexture = std::unordered_map<std::string, std::shared_ptr<Texture>>;
	using unorderedmapShader = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	using unorderedmapModel = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using unorderedmapMaterial = std::unordered_map<std::string, std::shared_ptr<Material>>;
	using unorderedmapObject3DGraph = std::unordered_map<std::string, std::shared_ptr<Object3DGraph>>;

	namespace Loader
	{


		/**
		  * Manage/Handle the resources loaded and storage.
		  * 
		  */
		BAKERS_API_CLASS ResourcesManager
		{

		private:
			/* the resources load*/
			unorderedmapShader				m_shaders;
			unorderedmapTexture				m_textures;
			unorderedmapMaterial			m_materials;
			unorderedmapModel				m_models;
			unorderedmapObject3DGraph		m_scenes;

			Core::Datastructure::RootObject* m_rootNode;


		public:
			Core::SystemManagement::TaskSystem m_task;
			/* Used for stocked the different value which allow to bind different resources to OpengGL (for the multiThread)*/
			std::list<std::shared_ptr<TextureData>>		m_texturesToLink;
			std::list<std::shared_ptr<ModelData>>		m_modelsToLink;
			std::list<std::shared_ptr<Material>>		m_materialsGetUniform;
			std::list< std::shared_ptr<ImporterData>>	m_importerToDelete;
		private:
			/**
			 * Load the 3D object in a single mesh for the obj and multiple mesh for the FBX, with the API assimp
			 * @param fileName: the name of the 3D object we want to load
			 */
			bool LoadAssimpScene(const char* fileName, const bool graphInMulti);

			const aiScene* LoadSceneFromImporter(Assimp::Importer& importer, const char* fileName);

			/**
			 * Load all the mesh in the 3D object and put the materials, 
			 * the models and the textures in the ResourcesManager
			 * @param importer: the importer who load the scene
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsScene(std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, const std::string& directory);
			
			/**
			 * Check if the mesh who is loading hasn't the same name (with path)
			 * as an other, and add number if it's the case
			 * @param currModelData: the current ModelData who will load the model
			 * @param currModel: the current Model who will be loaded
			 * @param nameMesh: the name of the mesh
			 * @return the number who is add to the path of the model = number of same name find
			 * (use after for the load of the aiMesh aiMaterial)
			 */
			unsigned int LoadMeshsSceneCheckModelIsLoaded(std::shared_ptr<ModelData>& currModelData, 
												std::shared_ptr<Model>& currModel, const std::string& nameMesh);

			/**
			 * Load all the meshs in the 3D object, 
			 * group them together inside one model 
			 * and put the materials, the model and the textures in the ResourcesManager
			 * @param importer: the importer who load the scene
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsSceneInSingleMesh(std::shared_ptr<Loader::ImporterData>& importer, 
											const aiScene* scene, const std::string& directory);

			/**
			 * Load the first Mesh of an obj, 
			 * and put the materials, the model and the textures in the ResourcesManager
			 * @param name: the name for find the object
			 * @param fileName: the name of the 3D object we want to load
			 */
			void LoadObjInModel(const std::string& name, const char* fileName);

			/**
			 * Load the Material of an aiMesh
			 * Check if the material hasn't been already load (or try), don't load if not
			 * @param importer: the importer who load the scene
			 * @param scene: the aiScene of the mesh
			 * @param mesh: the mesh of the material that we want load
			 * @param directory: the folder path of the 3D Object
			 * @param numberOfSameKey: an int default 0, 
			 * (just use in the function LoadMeshsScene) the number of time that the name of the mesh is used
			 */
			void LoadaiMeshMaterial(std::shared_ptr<Loader::ImporterData> importer, const aiScene* scene, aiMesh* mesh,
				const std::string& directory, const int numberOfSameKey = 0);
		public:
			/**
			 * Construct the ResourcesManager and create defaults shaders
			 */
			ResourcesManager();

			/**
			 * Remove all the resources and detach the link with OpenGL of all resources
			 */
			~ResourcesManager();

			/**
			 * Get the count in the textures unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the texture with which we want to know the count
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountTextures(const std::string& keyName) const
			{
				return m_textures.count(keyName);
			}

			/**
			 * Emplace a new texture in the textures unordered_map with a key
			 * Call the function emplace of unordered_map
			 * @param keyName: the key of the new texture
			 * @param texture: the shared_ptr of the texture that we want emplace
			 */
			inline void EmplaceTextures(std::string keyName, std::shared_ptr<Texture> texture)
			{
				m_textures.emplace(keyName, texture);
			}

			/**
			 * Get the count in the textures unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the texture with which we try to find
			 * @return the shared_ptr of the texture we try to find with the keyName
			 */
			inline std::shared_ptr<Texture> GetTexture(std::string keyName)
			{
				return m_textures[keyName];
			}

			/**
			 * Get the count in the materials unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the material with which we want to know the count
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountMaterials(const std::string& keyName) const
			{
				return m_materials.count(keyName);
			}

			/**
			 * Emplace a new material in the materials unordered_map with a key
			 * Call the function emplace of unordered_map
			 * @param keyName: the key of the new material 
			 * @param texture: the shared_ptr of the material that we want emplace
			 */
			inline void EmplaceMaterials(std::string keyName, std::shared_ptr<Material> texture)
			{
				m_materials.emplace(keyName, texture);
			}

			/**
			 * Get the count in the materials unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the material with which we try to find
			 * @return the shared_ptr of the material we try to find with the keyName
			 */
			inline std::shared_ptr<Material> GetMaterial(std::string keyName)
			{
				return m_materials[keyName];
			}

			/**
			 * Get the count in the models unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the model with which we want to know the count
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountModel(const std::string& keyName) const
			{
				return m_models.count(keyName);
			}

			/**
			 * Get the count in the models unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the model with which we try to find
			 * @return the shared_ptr of the model we try to find with the keyName
			 */
			inline std::shared_ptr<Model> GetModel(std::string keyName)
			{
				return m_models[keyName];
			}

			/**
			 * Get the count in the shaders unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the shader with which we try to find
			 * @return the shared_ptr of the shader we try to find with the keyName
			 */
			inline std::shared_ptr<Shader> GetShader(std::string keyName)
			{
				return m_shaders[keyName];
			}

			inline const unorderedmapShader& GetShaderMap()
			{
				return m_shaders;
			}

			/**
			 * Get the count in the scenes unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the scene with which we want to know the count
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountScene(const std::string& keyName)
			{
				return m_scenes.count(keyName);
			}

			/**
			 * Get the count in the scenes unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the scene with which we try to find
			 * @return the shared_ptr of the scene we try to find with the keyName
			 */
			inline std::shared_ptr<Object3DGraph> GetScene(const std::string& keyName)
			{
				return m_scenes[keyName];
			}

			/**
			 * Set access to Root Object of Scene
			 * @param r: Pointer to RootObject
			 */
			inline void SetRootNode(Core::Datastructure::RootObject* r)
			{
				m_rootNode = r;
			}

			inline void PushTextureToLink(std::shared_ptr<TextureData> textureData)
			{
				m_texturesToLink.push_back(textureData);
			}

			/**
			 * Check if the 3D object hasn't been already load (or try), and load it if not.
			 * @param fileName: the name of the 3D object we want to load
			 */
			void Load3DObject(const char* fileName, const bool graphInMulti = false);


			/**
			 * Load a texture
			 * Check if the texture hasn't been already load (or try to load), don't load if not
			 * @param keyName: the full folder path of the texture
			 * @param texture: shared_ptr of Texture
			 */
			void LoadTexture(const std::string& keyName, std::shared_ptr<Texture>& texture);

			/**
			 * Load a scene from an aiScene
			 * Check if the scene hasn't been already load (or try to load), don't load if not
			 * @param importer: the importer who load the scene
			 * @param scene: the aiScene we want to load
			 * @param keyName: the name of the scene it's the full folder path of the 3D object 
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadSceneResources(std::shared_ptr<Loader::ImporterData>& importer, 
									const aiScene* scene, const std::string& keyName, 
									const std::string& directory, const bool inMultiThread);

			/**
			 * Link to OpenGL the textures
			 */
			void LinkAllTextureToOpenGl();
			/**
			 * Link to OpenGL the models
			 */
			void LinkAllModelToOpenGl();

			void MaterialGetUniformFromShader();

			/*
			 * Check if the assimps importers can be destroy
			 */
			void CheckDeleteAssimpImporter();

			void UpdateResourcesManager();

			/**
			 * Create Shader and add it to the shader map
			 * @param shaderName: Name of the shader stored in ResourcesManager
			 * @param vertexFilePath: Path to the vertex shader file
			 * @param fragmentFilePath: Path to the fragment shader file
			 * @param header: Header type to add content to shader
			 * @return Pointer to create shader
			 */
			std::shared_ptr<Shader> CreateShader(const char* shaderName, const char* vertexFilePath, const char* fragmentFilePath, Shader::EShaderHeaderType header = Shader::EShaderHeaderType::NONE);

			/**
			 * Use each shader program and send lights components to shaders
			 */
			void ShaderUpdate();

			/**
			 * Reload each shader with current vertex and fragment shaders
			 */
			void ReloadShaders();

			/**
			 * Reload lua scripts linked to each scripted component
			 */
			void ReloadScripts();
		};

		/**
		  * cointains an assimp importer for the multiThread
		  * and a value for kill or let him be
		  */
		struct ImporterData
		{
			Assimp::Importer importer;
			unsigned int maxUseOfImporter{ 0 };
		};
	}
}