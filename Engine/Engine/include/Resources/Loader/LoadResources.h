#pragma once
#include <vector>
#include "Mesh.h"
#include "Vec3.hpp"
#include "Vec2.hpp"

//#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

struct aiScene;
struct aiMesh;
class Core::Datastructure::Object;

namespace Resources::Loader
{

	void LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, Core::Datastructure::Object* rootComponent = nullptr);

	void LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject, const bool newObjectChild = false);

	void LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene, std::string directory);

	Material LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, std::string directory);
}