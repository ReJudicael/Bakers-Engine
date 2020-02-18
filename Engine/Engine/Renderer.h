#pragma once

#include <vector>

#include "Mesh.h"

class Renderer
{
private:
	std::vector<Mesh*> m_meshes;
	GLuint m_program = 0;
	GLuint m_texture = 0;
	GLuint m_VAO = 0;
	GLuint m_vertexBuffer = 0;
	int		m_vertexCount = 0;
public:
	Renderer();
	~Renderer();

	GLuint CreateProgram(const char* vertex, const char* fragment);
	GLuint CreateTextureFromColor(const Core::Maths::Vec4& color);
	GLuint CreateTextureFromImage(const char* filename);

	void	Render();
	void	AddMesh(Mesh* newMesh);
	Mesh*	CreatePlane();
	Mesh*	CreateCube();
};

