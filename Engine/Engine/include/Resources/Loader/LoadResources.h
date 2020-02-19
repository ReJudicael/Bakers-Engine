#pragma once
#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"

//#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

struct aiScene;
namespace Resources::Loader
{
	struct vertex
	{
		Core::Maths::Vec3 Position;
		Core::Maths::Vec2 UV;
		Core::Maths::Vec3 Normal;
	};

	void loadResourcesIRenderable(Mesh* renderObject, const char* fileName);
	void loadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene);
}