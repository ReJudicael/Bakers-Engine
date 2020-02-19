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
	glDeleteBuffers(1, &m_vertexBuffer);
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

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices[0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
	glBindVertexArray(0);
}

void Mesh::OnDraw()
{
	glUseProgram(m_program);

	Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };
	//trs.mat.Print();
	std::cout << m_VAO << std::endl;
	glUniformMatrix4fv(glGetUniformLocation(m_program, "uModel"), 1, GL_FALSE, trs.m_array);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);
}

void Mesh::DrawMesh()
{
	glUseProgram(m_program);
	
	Core::Maths::Quaternion q(0, 0.01, 0);
	m_transform.Rotate(q);
	Core::Maths::Mat4 trs = m_transform.GetLocalTrs();
	/*for (int i = 0; i < 16; ++i)
		std::cout << trs.m_array[i] << " ; ";
	std::cout << std::endl;*/
	glUniformMatrix4fv(glGetUniformLocation(m_program, "uModel"), 1, GL_FALSE, trs.m_array);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Mesh* Mesh::CreateCube(const float offset, const float scale)
{
	Mesh* result = new Mesh();
	result->AddVertex(offset - scale, offset - scale, offset - scale - 1);
	result->AddVertex(offset - scale, offset - scale, offset + scale - 1);
	result->AddVertex(offset - scale, offset + scale, offset - scale - 1);
	result->AddVertex(offset - scale, offset + scale, offset + scale - 1);
	result->AddVertex(offset + scale, offset - scale, offset - scale - 1);
	result->AddVertex(offset + scale, offset - scale, offset + scale - 1);
	result->AddVertex(offset + scale, offset + scale, offset - scale - 1);
	result->AddVertex(offset + scale, offset + scale, offset + scale - 1);

	result->AddTriangle(0, 6, 4);
	result->AddTriangle(0, 2, 6);
	result->AddTriangle(0, 3, 2);
	result->AddTriangle(0, 1, 3);
	result->AddTriangle(2, 7, 6);
	result->AddTriangle(2, 3, 7);
	result->AddTriangle(4, 6, 7);
	result->AddTriangle(4, 7, 5);
	result->AddTriangle(0, 4, 5);
	result->AddTriangle(0, 5, 1);
	result->AddTriangle(1, 5, 7);
	result->AddTriangle(1, 7, 3);

	return result;
}