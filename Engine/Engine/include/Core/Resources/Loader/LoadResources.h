#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "ModelData.h"
#include "TextureData.h"
#include "Material.h"
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


namespace Resources
{
	struct Model;
	//struct Texture;
	//struct TextureData;

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
			void Load3DObject(const char* fileName);

			/**
			 * Load the 3D object in a single mesh for the obj and multiple mesh for the FBX, with the API assimp
			 * @param fileName: the name of the 3D object we want to load
			 */
			bool LoadAssimpScene(const char* fileName);

			/**
			 * Load all the mesh in the 3D object and put the materials, 
			 * the models and the textures in the ResourcesManager
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsScene(const aiScene* scene, const std::string& directory);
			

			int LoadMeshsSceneCheckModelIsLoaded(std::shared_ptr<ModelData>& currModelData, std::shared_ptr<Model>& currModel, const std::string& nameMesh);

			/**
			 * Load all the meshs in the 3D object, 
			 * group them together inside one model 
			 * and put the materials, the model and the textures in the ResourcesManager
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsSceneInSingleMesh(const aiScene* scene, const std::string& directory);


			void LoadObjInModel(const std::string& name, const char* fileName);

			/**
			 * Load the Material of an aiMesh
			 * Check if the material hasn't been already load (or try), don't load if not
			 * @param scene: the aiScene of the mesh
			 * @param mesh: the mesh of the material that we want load
			 * @param directory: the folder path of the 3D Object
			 * @param numberOfSameKey: an int default 0, 
			 * (just use in the function LoadMeshsScene) the number of time that the name of the mesh is used
			 */
			void LoadaiMeshMaterial(const aiScene* scene, aiMesh* mesh,
				const std::string& directory, const int numberOfSameKey = 0);

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
			 * @param scene: the aiScene we want to load
			 * @param keyName: the name of the scene it's the full folder path of the 3D object 
			 * param directory: the folder path of the 3D Object
			 */
			void LoadSceneResources(const aiScene* scene, const std::string& keyName, const std::string& directory);

			/**
			 * Link to OpenGL the textures
			 */
			void LinkAllTextureToOpenGl();

			/**
			 * Link to OpenGL the models
			 */
			void LinkAllModelToOpenGl();

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
	}
}