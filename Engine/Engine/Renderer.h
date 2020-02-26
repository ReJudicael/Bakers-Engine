#pragma once

#include <vector>

#include "Mesh.h"
#include "IRenderable.hpp"

class Renderer
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

