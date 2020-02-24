#pragma once
#include <vector>
#include <unordered_map>

#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "TextureData.h"
#include "ModelData.h"
#include "SceneData.h"

//#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

struct aiScene;
struct aiNode;
struct aiMesh;
class Core::Datastructure::Object;

namespace Resources::Loader
{

	class ResourcesManager
	{
	private :
		std::unordered_map<std::string, GLuint*>			m_shaders;
		std::unordered_map<std::string, TextureData*>		m_textures;
		std::unordered_map<std::string, ModelData*>			m_models;
		std::unordered_map<std::string, SceneData>			m_scenes;

	public :
		void LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, Core::Datastructure::Object* rootComponent = nullptr);

		void LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject, const bool newObjectChild = false);

		void LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene, const std::string& directory);

		Material LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, const std::string& directory);

		void LoadSceneResources(const aiScene* scene, const std::string& directory);

		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, Node& currentNode, const std::string& directory);
		//void DestroyResourcesManager();
	};

}