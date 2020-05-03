#pragma once
#include <vector>
#include "Vec3.hpp"
#include "CoreMinimal.h"

struct aiNode;
struct aiScene;

namespace Core::Datastructure
{
	class Object;
}

namespace Resources
{

	namespace Loader
	{
		class ResourcesManager;
	}

	struct Node
	{
		Core::Maths::Vec3	position;
		Core::Maths::Vec3	rotation;
		Core::Maths::Vec3	scale;

		std::string					nameMesh;
		std::string					nameObject;
		std::vector<std::string>	namesMaterial;

		std::vector<Node> children;

		/**
		 * Load a aiNode inside a Node
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 * @param directory: the full path of the first mesh find with bone
		 */
		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory);

		/**
		 * Load a assimp scene as one node
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 * @param directory: the full path of the first mesh find with bone
		 * @param isSkeletal: bool if the scene have a skelet
		 */
		void SingleMeshSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, const bool& isSkeletal);

		/**
		 * Load a assimp scene as a skeletal Object
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 * @param directory: the full path of the first mesh find with bone
		 * @param indexFirstMesh: the index of the first mesh who have a bone
		 */
		void Skeletal3DObjectLoad(const aiScene* scene, const aiNode* node, const std::string& directory, const unsigned int indexFirstMesh);

		/**
		 * Find in the scene a node with a mesh
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 */
		const aiNode* FindNodeWithMeshes(const aiScene* scene, const aiNode* node);

		/**
		 * create an object with a mesh from
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 */
		void CreateObjectScene(Core::Datastructure::Object* rootObject, Loader::ResourcesManager& resources,
								const bool isSingleMesh, const bool isSkeletal);

		/**
		 * create the mesh component
		 * @param Object: The object create by the Node
		 * @param resources: The resourcesmanager
		 * @param isSkeletal: true if the node is skeletal
		 */
		void ChooseRenderable(Core::Datastructure::Object* Object, Loader::ResourcesManager& resources, const bool isSkeletal);
	};

	struct Object3DGraph
	{
		Node rootNodeScene;
		bool singleMesh;
		bool haveSkeletal;

		/**
		 * Find in the scene a node with a mesh
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The rootNode of the assimp scene
		 * @param directory: the full path of the first mesh find with bone
		 */
		void	SceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory, const bool isSkeletal = false);

		/**
		 * Find in the scene a node with a mesh
		 * @param keyName: the name of the 3DObject
		 * @param resources: The resourcesmanager
		 * @param rootObject: The rootObject to assign the 3DObject
		 */
		static void CreateScene(const std::string& keyName, Loader::ResourcesManager& resources, 
									Core::Datastructure::Object* rootObject);
	};

}