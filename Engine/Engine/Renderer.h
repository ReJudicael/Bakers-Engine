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

	GLuint CreateProgram(const char* vertex, const char* fragment);
	GLuint CreateTextureFromColor(const Core::Maths::Vec4& color);
	GLuint CreateTextureFromImage(const char* filename, bool shouldFlip = true);

	void	Render();
	void	AddMesh(Mesh* newMesh);
	Mesh*	CreatePlane();
	Mesh*	CreateCube();
};

