#pragma once
#include <vector>
#include "Vec3.hpp"
#include "Debug.h"

struct aiNode;
struct aiScene;

namespace Resources
{
	struct Node
	{
		Core::Maths::Vec3	position;
		Core::Maths::Vec3	rotation;
		Core::Maths::Vec3	scale;

		std::string			nameMesh;

		std::vector<Node> children;

		void RecursiveSceneLoad(const aiScene* scene, const aiNode* node, const std::string& directory);
	};

	struct SceneData
	{
		Node rootNodeScene;
	};

}