#pragma once
#include <vector>
#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"

//#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

struct aiScene;
struct aiMesh;
namespace Resources::Loader
{

	void LoadResourcesIRenderable(Mesh* renderObject, const char* fileName);

	void LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene, std::string directory);

	unsigned int LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, std::string directory);
}