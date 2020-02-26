#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "ModelData.h"
#include "SceneData.h"
#include "Vertex.h" 

//#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class Core::Datastructure::Object;

namespace Resources::Loader
{

	class ResourcesManager
	{
	private :
		std::unordered_map<std::string, std::shared_ptr<GLuint>>			m_shaders;
		std::unordered_map<std::string, std::shared_ptr<TextureData>>		m_textures;
		std::unordered_map<std::string, std::shared_ptr<Material>>			m_materials;
		std::unordered_map<std::string, std::shared_ptr<ModelData>>			m_models;
		std::unordered_map<std::string, std::shared_ptr<SceneData>>			m_scenes;

		std::list<std::shared_ptr<TextureData>>								m_texturesToLink;
		std::list<std::shared_ptr<ModelData>>								m_modelsToLink;

		float* persperct;
	public :

		ResourcesManager();
		void LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, 
										Core::Datastructure::Object* rootComponent = nullptr);

		bool LoadAssimpScene(Mesh* renderObject, const char* fileName,
				Core::Datastructure::Object* rootComponent = nullptr);

		void CreateScene(std::shared_ptr<SceneData> scene, Core::Datastructure::Object* rootObject);
		void RecurciveCreateScene(const Node& scene, Core::Datastructure::Object* rootObject);

		void LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject, 
										const bool newObjectChild = false);


		void LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene, const std::string& fileName, 
												const std::string& directory);
		void LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, std::shared_ptr<Material> material,
												const std::string& directory);
		void LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<TextureData> textureData, const aiTextureType& textureType, const std::string& directory);

		void LoadSceneResources(const aiScene* scene, const std::string& fileNameconst,const std::string& directory);
		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, Node& currentNode, const std::string& directory);

		void CreateTextureFromImage(const char* filename, std::shared_ptr<TextureData> textureData, bool shouldFlip = true);

		void linkAllTextureToOpenGl();
		void linkAllModelToOpenGl();

		void CreateProgram(const char* vertex, const char* fragment, const std::string& nameShader);
	};

}