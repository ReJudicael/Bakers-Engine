#include <iostream>
#include <glad/glad.h>
#include "Mesh.h"
#include "Mat4.hpp"
#include "Object.hpp"
#include "RootObject.hpp"
#include "OpenGLLinkState.h"
#include "Model.h"
#include "Texture.h"
#include "LoadResources.h"
#include "PhysicsScene.h"
#include "EngineCore.h"
#include "PxRigidStatic.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Mesh>("Mesh")
		.constructor()
		.property_readonly("Vertex Count", &Mesh::GetVertexCount);
}

Mesh::Mesh() : ComponentBase(), m_projection{ nullptr }
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
	return 0;
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

bool Mesh::IsModelLoaded()
{
	if (m_model->stateVAO == Resources::EOpenGLLinkState::ISLINK)
		return true;
	return false;
}

void Mesh::CreateAABBMesh()
{
	Core::Datastructure::Object* object = GetParent();
	
	GetScene()->GetEngine()->AddMeshToNav(m_model->vertices.data(), m_model->vertices.size(), m_model->indices.data(), m_model->indices.size(), object->GetUpdatedTransform());
	
	Core::Datastructure::IComponent* component = dynamic_cast<Core::Datastructure::IComponent*>(this);
	void* testCast = object->GetScene()
							->GetEngine()
							->GetPhysicsScene()
							->CreateEditorPhysicsActor(static_cast<void*>(component), object->GetUpdatedTransform(),m_model)->userData;
	component = static_cast<Core::Datastructure::IComponent*>(testCast);
	Mesh* meshComp = static_cast<Mesh*>(testCast);
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

	glBindVertexArray(m_model->VAOModel);

	for (int i = 0; i < m_model->offsetsMesh.size(); i++)
	{
		Resources::OffsetMesh currOffsetMesh = m_model->offsetsMesh[i];
		
		Resources::Material material = *m_materialsModel[currOffsetMesh.materialIndices];
		material.shader->UseProgram();
		{

			// init the value of the texture1
			glUniform1i(material.shader->GetLocation("uColorTexture"), 0);
			// init the value of the texture2
			glUniform1i(material.shader->GetLocation("uNormalMap"), 1);

			//material.shader->SendLights();

			material.SendMaterial();
			glUniformMatrix4fv(material.shader->GetLocation("uModel"), 1, GL_TRUE, trs.m_array);
			glUniformMatrix4fv(material.shader->GetLocation("uCam"), 1, GL_TRUE, cam->GetCameraMatrix().m_array);
			glUniformMatrix4fv(material.shader->GetLocation("uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().m_array);
		}

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
			if (material.textures.size() >= 2 && material.textures[1]->stateTexture ==
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

void Mesh::AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial)
{
	for (int i{ 0 }; i < m_model->offsetsMesh.size(); i++)
	{
		m_materialsModel.push_back(resources.GetMaterial(namesMaterial[i]));
	}
}

int Mesh::GetVertexCount()
{
	int size = 0;
	for (int i = 0; i < m_model->offsetsMesh.size(); ++i)
		size += m_model->offsetsMesh[i].count;
	return size;
}

void	Mesh::OnReset()
{
	ComponentBase::OnReset();
	IRenderable::OnReset();
}
