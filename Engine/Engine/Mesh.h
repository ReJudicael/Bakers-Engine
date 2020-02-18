#pragma once

#include <GLFW/glfw3.h>

#include "Vec3.hpp"

class Mesh
{
public:
	GLuint m_program = 0;
	GLuint m_texture = 0;
	GLuint m_VAO = 0;
	GLuint m_vertexBuffer = 0;
	int		m_vertexCount = 0;

	Core::Maths::Vec3 translate = { 0.f, 0.f, 0.f };
	Core::Maths::Vec3 rotate = { 0.f, 0.f, 0.f};
	Core::Maths::Vec3 scale = { 1.f, 1.f, 1.f };

public:
	Mesh();
	~Mesh();

	void DrawMesh();
};

