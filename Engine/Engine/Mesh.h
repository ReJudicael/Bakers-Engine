#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Vec3.hpp"
#include "Transform.hpp"

class Mesh
{
public:
	GLuint m_program = 0;
	GLuint m_texture = 0;
	GLuint m_VAO = 0;
	GLuint m_vertexBuffer = 0;
	GLuint m_indicesBuffer = 0;
	int		m_vertexCount = 0;

	std::vector<float> m_vertices;
	std::vector<int> m_indices;

	Core::Datastructure::Transform	m_transform;

	int		AddTriangle(int v1, int v2, int v3);
	int		AddVertex(float x, float y, float z);

public:
	Mesh();
	~Mesh();

	void Initialize();

	void DrawMesh();

	static Mesh* CreateCube(const float offset, const float scale);
};

