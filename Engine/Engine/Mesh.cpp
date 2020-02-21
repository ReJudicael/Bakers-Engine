#include <iostream>
#include <glad/glad.h>

#include "Mesh.h"
#include "Mat4.hpp"
#include "Object.hpp"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	glDeleteTextures(1, &m_texture);
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteProgram(m_program);
}

int Mesh::AddTriangle(int v1, int v2, int v3)
{
	if (v1 >= m_vertices.size() / 3 || v2 >= m_vertices.size() / 3 || v3 >= m_vertices.size() / 3)
		return -1;
	m_indices.push_back(v1);
	m_indices.push_back(v2);
	m_indices.push_back(v3);
}

int Mesh::AddVertex(float x, float y, float z)
{
	int pos{ m_vertices.size() % 3 };
	m_vertices.push_back(x);
	m_vertices.push_back(y);
	m_vertices.push_back(z);
	return pos;
}

void Mesh::Initialize()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices[0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
	glBindVertexArray(0);
}

void Mesh::SendProjectionMatrix(Core::Maths::Mat4 data)
{
	m_projection = data.m_array;
}

void Mesh::OnDraw()
{
	Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };

	glUniformMatrix4fv(glGetUniformLocation(m_program, "uModel"), 1, GL_FALSE, trs.m_array);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "uProj"), 1, GL_FALSE, m_projection);

	glUseProgram(m_program);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}