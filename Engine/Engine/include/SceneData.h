#pragma once
#include <vector>
#include "Vec3.hpp"


namespace Resources
{
	struct Node
	{
		Core::Maths::Vec3	position;
		Core::Maths::Vec3	rotation;
		Core::Maths::Vec3	scale;

		std::string			nameMesh;

		std::vector<Node> children;
	};

	struct SceneData
	{
		Node rootNodeScene;
	};
}