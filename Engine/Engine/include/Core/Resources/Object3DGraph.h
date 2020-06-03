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
		struct ImporterData;
	}

	struct Object3DGraph;

	/**
	 * extract value of a aiNode, like position, rotation, name,
	 * model etc..
	 */
	struct Node
	{
		Core::Maths::Vec3	position;
		Core::Maths::Vec3	rotation;
		Core::Maths::Vec3	scale;

		std::string					nameMesh;
		std::string					nameObject;
		std::vector<std::string>	namesMaterial;
		bool						isSkeletal{ false };

		std::vector<Node> children;

		/**
		 * Load a aiNode inside a Node
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the 
		 * @param directory: the path of the 3D Object
		 */
		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, 
								const std::string& directory);

		/**
		 * Load all the aiMesh of the aiNode as one Node
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the 
		 * @param mat: a aiMaterial pointeur
		 * @param directory: the path of the 3D Object
		 */
		void LoadMeshsAsChild(	const aiScene* scene, const aiNode* node, aiMaterial* mat, 
								const std::string& directory);

		/**
		 * Find the frist Node with a Meshes
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The node with wich we try to find the first mesh
		 */
		const aiNode* FindNodeWithMeshes(const aiScene* scene, const aiNode* node);

		/**
		 * Load a assimp scene as one node
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 * @param directory: the path of the 3D Object
		 */
		void SingleMeshSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory);

		/**
		 * create an object with a mesh from
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The current node of the scene
		 */
		void CreateObjectScene(Core::Datastructure::Object* rootObject, Loader::ResourcesManager& resources);
	};

	/**
	 * Constains the node hierarchy
	 */
	struct Object3DGraph
	{
		Node rootNodeScene;
		bool singleMesh;

		/**
		 * Find in the scene a node with a mesh
		 * @param scene: The scene of the 3D object load by assimp
		 * @param node: The rootNode of the assimp scene
		 * @param directory: the path of the 3D Object
		 * @param singleMesh: true if the 3D Object is consider as a single Mesh
		 */
		void	SceneLoad(std::shared_ptr<Resources::Loader::ImporterData>& importer, const aiScene* scene, const aiNode* node, 
							const std::string& directory, const bool singleMesh);

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