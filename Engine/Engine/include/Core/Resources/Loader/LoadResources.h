#pragma once
#include <vector>
#include <list>
#include <memory>
#include "Assimp/Importer.hpp"

#include "CoreMinimal.h"
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
#include "Audio/AudioClip.h"
#include "BoneData.h"
#include "Bone.h"
#include "Animation.h"
#include "Character.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType : int;
class Core::Datastructure::Object;


namespace Resources
{

	struct Model;

	using unorderedmapTexture = std::unordered_map<std::string, std::shared_ptr<Texture>>;
	using unorderedmapShader = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	using unorderedmapModel = std::unordered_map<std::string, std::shared_ptr<Model>>;
	using unorderedmapMaterial = std::unordered_map<std::string, std::shared_ptr<Material>>;
	using unorderedmapObject3DGraph = std::unordered_map<std::string, std::shared_ptr<Object3DGraph>>;
	using unorderedmapBonesIndex = std::unordered_map<std::string, Animation::BoneData>;
	using unorderedmapBonesHierarchy = std::unordered_map<std::string, std::shared_ptr<Core::Animation::BoneTree>>;
	using unorderedmapAnimations = std::unordered_map<std::string, std::shared_ptr<Core::Animation::Animation>>;
	using unorderedmapAudioClip = std::unordered_map<std::string, std::shared_ptr<Core::Audio::AudioClip>>;

	namespace Loader
	{
		BAKERS_API_STRUCT Object3DInfo
		{
			std::vector<std::string> modelsName;
			std::vector<std::string> materialsName;
		};

		/**
		  * Manage/Handle the resources loaded and storage.
		  * 
		  */
		BAKERS_API_CLASS ResourcesManager
		{

		private:
			/* the resources load*/
			unorderedmapShader					m_shaders;
			unorderedmapTexture					m_textures;
			unorderedmapMaterial				m_materials;
			unorderedmapModel					m_models;
			unorderedmapObject3DGraph			m_scenes;
			unorderedmapBonesIndex				m_bonesID;
			unorderedmapBonesHierarchy			m_BoneHierarchies;
			unorderedmapAudioClip				m_audioClips;
			Core::Datastructure::RootObject*	m_rootNode;
			std::map<std::string, std::pair<std::map<GLchar, Character*>, FT_Face>*>	m_fontMap;

			FT_Library																	m_ft;
		public:
			Core::SystemManagement::TaskSystem*			m_task;
			unorderedmapAnimations						m_animations;
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
			bool LoadAssimpScene(const char* fileName, Object3DInfo& info, const bool graphInMulti);

			const aiScene* LoadSceneFromImporter(Assimp::Importer& importer, const char* fileName);

			/**
			 * Load all the mesh in the 3D object and put the materials, 
			 * the models and the textures in the ResourcesManager
			 * @param importer: the importer who load the scene
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsScene(std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, const std::string& directory, Object3DInfo& info);
			
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
												std::shared_ptr<Model>& currModel, std::string& nameMesh);

			/**
			 * Load all the meshs in the 3D object, 
			 * group them together inside one model 
			 * and put the materials, the model and the textures in the ResourcesManager
			 * @param importer: the importer who load the scene
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadMeshsSceneInSingleMesh(std::shared_ptr<Loader::ImporterData>& importer, 
											const aiScene* scene, const std::string& directory, Object3DInfo& info);

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
			void LoadaiMeshMaterial(	std::shared_ptr<Loader::ImporterData> importer, const aiScene* scene, aiMesh* mesh,
										const std::string& directory, Object3DInfo& info, const int numberOfSameKey = 0);
		public:
			/**
			 * Default constructor 
			 * Construct the ResourcesManager and create defaults shaders
			 */
			ResourcesManager();
			/**
			 * Constructor for link the task system to the ResourcesManager
			 * @param task: a pointeur on the task system
			 */
			ResourcesManager(Core::SystemManagement::TaskSystem* task);

			void DefaultConstruct();

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
			 * Get the shared_ptr in the textures unordered_map of the key given
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
			 * Get the shared_ptr in the materials unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the material with which we try to find
			 * @return the shared_ptr of the material we try to find with the keyName
			 */
			inline std::shared_ptr<Material> GetMaterial(std::string keyName)
			{
				if (GetCountMaterials(keyName) > 0)
					return m_materials[keyName];
				else if (keyName.find(".bmat") != std::string::npos)
					return LoadBMatMaterial(keyName);
				return nullptr;
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
			 * Get the shared_ptr in the models unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the model with which we try to find
			 * @return the shared_ptr of the model we try to find with the keyName
			 */
			inline std::shared_ptr<Model> GetModel(std::string keyName)
			{
				return m_models[keyName];
			}

			/**
			 * Get the shared_ptr in the shaders unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the shader with which we try to find
			 * @return the shared_ptr of the shader we try to find with the keyName
			 */
			std::shared_ptr<Shader> GetShader(std::string keyName)
			{
				if (GetCountShader(keyName) > 0)
					return m_shaders[keyName];
				else if (keyName.find(".bshader") != std::string::npos)
					return LoadBShaderShader(keyName);
				return nullptr;
			}

			/**
			 * Get the count in the shaders unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the shader with which we try to find
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountShader(std::string keyName)
			{
				return m_shaders.count(keyName);
			}

			/**
			 * Shader map getter
			 * @return Map storing all shaders
			 */
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
			/**
			 * Get the count in the skeleton unordered_map of the key given
			 * Call the function count of unordered_map
			 * @param keyName: the key of the skeleton with which we want to know the count
			 * @return the number of elements wich have the key
			 */
			inline size_t GetCountSkeleton(const std::string& keyName)
			{
				return m_BoneHierarchies.count(keyName);
			}
			/**
			 * Get the count in the skeleton unordered_map of the key given
			 * Call the operator [] of unordered_map
			 * @param keyName: the key of the skeleton with which we try to find
			 * @return the shared_ptr of the skeleton we try to find with the keyName
			 */
			inline std::shared_ptr<Core::Animation::BoneTree> GetSkeleton(const std::string& keyName)
			{
				return m_BoneHierarchies[keyName];
			}

			/**
			 * Push in the list of textureToLink a TextureData 
			 * @param textureData: the textureData to push
			 */
			inline void PushTextureToLink(std::shared_ptr<TextureData> textureData)
			{
				m_texturesToLink.push_back(textureData);
			}

			/**
			 * Create a new Material as a default material
			 * @param path: the path of the material
			 */
			void CreateNewMaterial(const std::string& path);

			/**
			 * Replace the material in the materials map
			 * changing the path
			 * @param path: the path of the material
			 * @param newPath: the newPath who replace path
			 */
			void ReplaceMaterial(const std::string& path, const std::string& newPath);

			/**
			 * Load a material with a .bmat
			 * @param path: the path of the file
			 */
			std::shared_ptr<Resources::Material> LoadBMatMaterial(const std::string& path);

			/**
			 * Create a new Shader as a default shader
			 * @param path: the path of the Shader
			 */
			void CreateNewShader(const std::string& path);

			/**
			 * Replace the material in the shader map
			 * changing the path
			 * @param path: the path of the shader
			 */
			void ReplaceShader(const std::string& path, const std::string& newPath);

			/**
			 *
			 */
			void UpdateShaderPathInMaterials(std::shared_ptr<Resources::Shader> shader);

			/**
			 * Load a material with a .bmat
			 * @param path: the path of the file
			 */
			std::shared_ptr<Resources::Shader> LoadBShaderShader(const std::string& path);

			/**
			 * Check if the 3D object hasn't been already load (or try), and load it if not.
			 * @param fileName: the name of the 3D object we want to load
			 */
			Object3DInfo Load3DObject(const char* fileName, const bool graphInMulti = false);

			/**
			 * Load the bones of the meshses of a 3DObject for have one mesh
			 * @param modelData: The modelData who have put the value of bones for the VAO
			 * @param scene: The scene of the 3D object load by assimp
			 * @param mesh: The current mesh who is loading
			 * @param directory: the full path of the first mesh find with bone
			 * @param numBones: the number of bone of all the meshes, increase inside the function
			 * @param numVertices: The current number of vertices since the first to the current meshes who is loading 
			 * @param bonesIndex: the unordered_map of the bone index of the current assimp scene
			 */
			void LoadAnimationaiMesh(	std::shared_ptr<ModelData>& modelData, const aiScene* scene, 
										aiMesh* mesh, const std::string& directory
										, unsigned int& numBones, const unsigned int& numVertices, 
										std::shared_ptr<unorderedmapBonesIndex>& bonesIndex);

			/**
			 * Load the hierarchy of bones
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the full path of the first mesh find with bone
			 * @param numBones: the number of bone of all the meshes, increase inside the function
			 * @param bonesIndex: the unordered_map of the bone index of the current assimp scene
			 */
			void LoadBonesHierarchy(const aiScene* scene, const std::string& directory
									, const unsigned int& numBones
									, const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex);

			/**
			 * Find in the nodes scene the first node who is a bone
			 * @param node: The current node of the scene
			 * @param bonesIndex: the unordered_map of the bone index of the current assimp scene
			 * @param mat: the matrix
			 */
			const aiNode* FindFirstBoneNode(const aiNode* node, 
											const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex);

			/**
			 * Load a file as an animation
			 * @param fileName: the path of the name
			 */
			std::shared_ptr<Core::Animation::Animation> LoadAsAnAnimation(const std::string& fileName);

			/**
			 * Load all the animation of an assimp scene
			 * @param scene: The scene of the 3D object load by assimp
			 * @param directory: the folder path of the 3D Object
			 */
			void LoadAnimation(const aiScene* scene, const std::string& directory, const std::string& fileName);

			/**
			 * Load a texture
			 * Check if the texture hasn't been already load (or try to load), don't load if not
			 * @param keyName: the full folder path of the texture
			 * @param texture: shared_ptr of Texture
			 */
			void LoadTexture(const std::string& keyName, std::shared_ptr<Texture>& texture);

			/**
			 * Load a Object3DGraph from an aiScene
			 * Check if the scene hasn't been already load (or try to load), don't load if not
			 * @param importer: the importer who load the scene
			 * @param scene: the aiScene we want to load
			 * @param keyName: the name of the scene it's the full folder path of the 3D object 
			 * @param directory: the folder path of the 3D Object
			 * @param inMultiThread: whether the resources are loaded in multithread or not
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

			/**
			 * the materials who have a shader get all the uniform for his shader
			 */
			void MaterialGetUniformFromShader();

			/**
			 * Check if the assimps importers can be destroy
			 */
			void CheckDeleteAssimpImporter();

			/**
			 * Update the value of the ResourcesManager
			 */
			void UpdateResourcesManager();

			std::shared_ptr<Core::Audio::AudioClip> CreateAudioClip(const std::string& audioClipPath, FMOD::System* fmodSystem);

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

			/**
			 * Save all the special materials
			 * @param shaderPTR: the shader we want to find the path
			 * @return the path of the Shader
			 */
			std::string FindPathFromShared(std::shared_ptr<Resources::Shader> shaderPTR);
			/**
			 * Save all the special materials
			 * @param materialPTR: the material we want to find the path
			 * @return the path of the Material
			 */
			std::string FindPathFromShared(std::shared_ptr<Resources::Material> materialPTR);
			/**
			 * Save all the special materials
			 * @param texturePTR: the texture we want to find the path
			 * @return the path of the texture
			 */
			std::string FindPathFromShared(std::shared_ptr<Resources::Texture> texturePTR);

			/**
			 * Save all the special materials
			 */
			void SaveMaterial();

			/**
			 * Update all materials who have this shader who is going to be deleted
			 * and change the material shader as default
			 * @param shaderDeleted: the shader deleted
			 */
			void UpdateDeletedShaderMaterials(const std::shared_ptr<Resources::Shader>& shaderDeleted);

			/**
			 * Delete the material and the shader create by ".bmat" and ".bshader"
			 * @param path: the path of the shader othe material to delete
			*/
			void DeleteSpecialMaterialShader(const std::string& path);

			Character* GetCharacter(const std::string& fontPath, char c);
			std::pair<std::map<GLchar, Character*>, FT_Face>* GetFont(const std::string& fontPath);
		};

		/**
		 * contains an assimp importer for the multiThread
		 * and a value for kill or let him be
		 */
		struct ImporterData
		{
			Assimp::Importer importer;
			unsigned int maxUseOfImporter{ 0 };
		};
	}
}