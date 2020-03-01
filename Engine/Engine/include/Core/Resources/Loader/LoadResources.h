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
#include "Debug.h"


struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class Core::Datastructure::Object;

namespace Resources
{
	struct Model;
	struct Texture;
	struct TextureData;
	//struct SceneData;

	namespace Loader
	{
		class ResourcesManager
		{
		private:
			std::unordered_map<std::string, std::shared_ptr<GLuint>>			m_shaders;
			std::unordered_map<std::string, std::shared_ptr<Texture>>			m_textures;
			std::unordered_map<std::string, std::shared_ptr<Material>>			m_materials;
			std::unordered_map<std::string, std::shared_ptr<Model>>				m_models;
			std::unordered_map<std::string, std::shared_ptr<SceneData>>			m_scenes;


		public:


			std::list<std::shared_ptr<TextureData>>								m_texturesToLink;
			std::list<std::shared_ptr<ModelData>>								m_modelsToLink;

			ResourcesManager();
			~ResourcesManager();

			inline int GetCountTextures(const std::string& str) const
			{
				return m_textures.count(str);
			}

			inline void EmplaceTextures(std::string name, std::shared_ptr<Texture> texture)
			{
				m_textures.emplace(name, texture);
			}

			inline std::shared_ptr<Texture> GetTexture(std::string path)
			{
				return m_textures[path];
			}

			void LoadResourcesIRenderable(const char* fileName,
				Core::Datastructure::Object* rootComponent);

			bool LoadAssimpScene(const char* fileName,
				Core::Datastructure::Object* rootComponent);

			void CreateScene(std::shared_ptr<SceneData> scene, Core::Datastructure::Object* rootObject);
			void RecurciveCreateScene(const Node& scene, Core::Datastructure::Object* rootObject);

			void LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject,
				const bool newObjectChild);

			void LoadMeshResourcesIRenderable(const aiScene* scene,
				const std::string& fileName, const std::string& directory);

			void LoadSingleMeshResourcesIRenderable(const aiScene* scene, const std::string& fileName,
				const std::string& directory);

			void LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, std::shared_ptr<Material>& material,
				const std::string& directory, const int meshNameCall = 0);
			bool LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& textureData, const aiTextureType& textureType, const std::string& directory);

			void LoadSceneResources(const aiScene* scene, const std::string& fileNameconst, const std::string& directory);

			void LinkAllTextureToOpenGl();
			void LinkAllModelToOpenGl();

			void CreateProgram(const char* vertex, const char* fragment, const std::string& nameShader);
		};
	}
}