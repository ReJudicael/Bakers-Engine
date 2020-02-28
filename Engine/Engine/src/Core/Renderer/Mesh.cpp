#include <iostream>
#include <glad/glad.h>

#include "Mesh.h"
#include "Mat4.hpp"
#include "Object.hpp"
#include "OpenGLLinkState.h"
#include "Model.h"
#include "Texture.h"

Mesh::Mesh() : ComponentBase()
{

}

Mesh::~Mesh()
{
	glDeleteTextures(1, &m_texture);
	//glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_VAO);
	//glDeleteProgram(m_program);
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

	/*glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices[0], GL_STATIC_DRAW);*/

	/*glGenBuffers(1, &m_indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices[0], GL_STATIC_DRAW);*/

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

void Mesh::OnDraw(Core::Datastructure::ICamera* cam)
{

	Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };

	// check if the mesh have a modelMesh
	if (m_model == nullptr)
		return;
	// check if the VAO of the model is link to OpenGL
	if (m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
		return;

	glEnable(GL_DEPTH_TEST);

	// init the value of the texture1
	glUniform1i(glGetUniformLocation(*m_program, "uColorTexture"), 0);
	// init the value of the texture2
	glUniform1i(glGetUniformLocation(*m_program, "uNormalMap"), 1);

	glUniformMatrix4fv(glGetUniformLocation(*m_program, "uModel"), 1, GL_TRUE, trs.m_array);
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "uCam"), 1, GL_TRUE, cam->GetCameraMatrix().m_array);
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().m_array);

	glBindVertexArray(m_model->VAOModel);

	glUseProgram(*m_program);
	

	for (int i = 0; i < m_model->offsetsMesh.size(); i++)
	{
		Resources::OffsetMesh currOffsetMesh = m_model->offsetsMesh[i];
		
		Resources::Material material = *m_model->materialsModel[currOffsetMesh.materialIndices];

		// check if the material have a texture
		if (material.textures.size() > 0)
		{
			// check if the texture1 link to OpenGL
			if (material.textures[0]->stateTexture ==
				Resources::EOpenGLLinkState::ISLINK)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material.textures[0]->texture);
			}
			// check if the texture2 link to OpenGL
			if (material.textures[1]->stateTexture ==
				Resources::EOpenGLLinkState::ISLINK)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material.textures[1]->texture);
			}
		}

		glDrawElements(GL_TRIANGLES, currOffsetMesh.count, GL_UNSIGNED_INT,
			(GLvoid*)(currOffsetMesh.beginIndices * sizeof(GLuint)));
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

Core::Maths::Mat4 Mesh::projectionMatrix(float FovY, float Aspect, float Near, float Far)
{
	float Top = Near * tanf(FovY / 2.f);
	float Right = Top * Aspect;

	float f[16] {
		(Near * 2.f) / (Right - (-Right)), 0.f, 0.f, 0.f,
		0.f, (Near * 2.f) / (Top - (-Top)), 0.f, 0.f,
		(Right + (-Right)) / (Right - (-Right)), (Top + (-Top)) / (Top - (-Top)), -(Far + Near) / (Far - Near), -1.f,
		0.f, 0.f, -(Far * Near * 2.f) / (Far - Near), 0.f
		};
	return Core::Maths::Mat4(f);
}