#pragma once
#include <vector>
#include "Vec3.hpp"
#include "Debug.h"

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

		std::string			nameMesh;

		std::vector<Node> children;

		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory);

		void CreateObjectScene(Core::Datastructure::Object* rootObject, Loader::ResourcesManager& resources);
	};

	struct SceneData
	{
		Node rootNodeScene;


		static void CreateScene(const std::string& keyName, Loader::ResourcesManager& resources, Core::Datastructure::Object* rootObject);
	};

}