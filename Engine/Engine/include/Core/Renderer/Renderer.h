#pragma once

#include <vector>

#include "Mesh.h"
#include "IRenderable.hpp"
#include "CoreMinimal.h"

BAKERS_API_CLASS Renderer
{
private:
	std::vector<Core::Datastructure::IRenderable*> m_meshes;
public:
	Renderer();
	~Renderer();

	GLuint CreateTextureFromColor(const Core::Maths::Vec4& color);

	void	AddMesh(Mesh* newMesh);
	Mesh*	CreatePlane();
	Mesh*	CreateCube();
};