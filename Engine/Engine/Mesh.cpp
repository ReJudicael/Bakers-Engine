#include <iostream>
#include <glad/glad.h>

#include "Mesh.h"
#include "Mat4.hpp"

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

void Mesh::DrawMesh()
{
	glUseProgram(m_program);
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindVertexArray(m_VAO);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	glBindVertexArray(0);
}