#include <iostream>
#include <glad/glad.h>

#include "PxRigidActor.h"

#include "Mesh.h"
#include "Mat4.hpp"
#include "Object.hpp"
#include "RootObject.hpp"
#include "OpenGLLinkState.h"
#include "Texture.h"
#include "LoadResources.h"
#include "PhysicsScene.h"
#include "EngineCore.h"
#include "PxRigidActor.h"
#include "TriggeredEvent.h"
#include "Object3DGraph.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Mesh>("Mesh")
		.constructor()
		.property("Model", &Mesh::GetModel, &Mesh::SetModel)
		.property("IsRoot", &Mesh::m_isRoot, detail::protected_access())
		.property("MaterialsNames", &Mesh::m_materialsNames, detail::protected_access())
		.property("IsChild", &Mesh::m_isChild, detail::protected_access());
}

void Mesh::SetModel(std::string newModel)
{
	if (m_modelName != "")
	{
		m_isRoot = false;
		m_isChild = false;
	}
	m_modelName = newModel;
	if (!IsInit())
		return;
	UpdateModel();
}

void Mesh::SetChildModel(std::string newModel)
{
	m_isRoot = false;
	m_isChild = true;
	m_modelName = newModel;
	if (!IsInit())
		return;
	UpdateModel();
}

void Mesh::UpdateModel()
{
	if (m_modelName == "")
		return;
	
	Resources::Loader::ResourcesManager* manager{ GetRoot()->GetEngine()->GetResourcesManager() };
	manager->Load3DObject(m_modelName.c_str());

	if (m_isChild)
	{
		if (manager->GetCountModel(m_modelName) <= 0)
			return;
		AddModel(manager->GetModel(m_modelName));

		AddMaterials(*manager, m_materialsNames);

		//std::shared_ptr<Resources::Object3DGraph> scene{ manager->GetScene(m_modelName) };
		//AddMaterials(*manager, scene->rootNodeScene.namesMaterial);
	}
	else
	{
		if (manager->GetCountScene(m_modelName) <= 0)
			return;
		std::shared_ptr<Resources::Object3DGraph> scene{ manager->GetScene(m_modelName) };
		if (!scene)
		{
			BAKERS_LOG_WARNING("Scene loading failed");
			return;
		}
		if (scene->singleMesh)
		{
			m_isRoot = false;
			AddModel(manager->GetModel(scene->rootNodeScene.nameMesh));
			AddMaterials(*manager, scene->rootNodeScene.namesMaterial);
		}
		else
		{
			if (m_isRoot)
				return;
			m_isRoot = true;
			for (auto i{ 0 }; i < scene->rootNodeScene.children.size(); i++)
			{
				Core::Datastructure::Object* childObject{ GetParent()->CreateChild("", {}) };
				scene->rootNodeScene.children[i].CreateObjectScene(childObject, *manager);
			}
		}
	}
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

void Mesh::OnInit()
{
	IRenderable::OnInit();
	UpdateModel();
}

void Mesh::OnDestroy()
{
	m_destroyActorEvent.Invoke();
	IRenderable::OnDestroy();
}

void Mesh::SendProjectionMatrix(Core::Maths::Mat4 data)
{
	m_projection = data.array;
}

bool Mesh::IsModelLoaded()
{
	return m_model && m_model->stateVAO == Resources::EOpenGLLinkState::ISLINK;
}

void Mesh::CreateAABBMesh()
{
	Core::Datastructure::Object* object = GetParent();
	
	GetRoot()->GetEngine()->AddMeshToNav(m_model->vertices.data(), static_cast<int>(m_model->vertices.size()), m_model->indices.data(), static_cast<int>(m_model->indices.size()), object->GetUpdatedTransform());
	
	Core::Datastructure::IComponent* component = dynamic_cast<Core::Datastructure::IComponent*>(this);
	physx::PxRigidActor* actor = object->GetScene()
							->GetEngine()
							->GetPhysicsScene()
							->CreateEditorPhysicsActor(static_cast<void*>(component), object->GetUpdatedTransform(), m_model);
	
	m_destroyActorEvent += std::bind(&Core::Physics::PhysicsScene::DestroyEditorPhysicActor , object->GetScene()
		->GetEngine()->GetPhysicsScene(), actor);

	GetParent()->SetAnEventTransformChange(std::bind(&Core::Physics::PhysicsScene::UpdatePoseOfActor, object->GetScene()
											->GetEngine()->GetPhysicsScene(), actor));
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
			glUniformMatrix4fv(material.shader->GetLocation("uModel"), 1, GL_TRUE, trs.array);
			glUniformMatrix4fv(material.shader->GetLocation("uCam"), 1, GL_TRUE, cam->GetCameraMatrix().array);
			glUniformMatrix4fv(material.shader->GetLocation("uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().array);
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

void Mesh::AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial)
{
	if (!m_model)
	{
		m_materialsNames = namesMaterial;
	}
	else
	{
		for (int i{ 0 }; i < m_model->offsetsMesh.size(); i++)
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

void Mesh::OnCopy(IComponent* copyTo) const
{
	ComponentBase::OnCopy(copyTo);
	IRenderable::OnCopy(copyTo);
}

void Mesh::StartCopy(IComponent*& copyTo) const
{
	copyTo = new Mesh();
	OnCopy(copyTo);
}

bool Mesh::OnStart()
{
	if (IsModelLoaded())
	{
		CreateAABBMesh();
		IRenderable::OnStart();
		ComponentBase::OnStart();
		return true;
	}
	return false;
}

std::shared_ptr<Resources::Material> Mesh::GetMainMaterial()
{
	if (m_materialsModel.size() == 0)
		return nullptr;
	else
		return m_materialsModel[0];
}

void Mesh::SetMainMaterial(std::shared_ptr<Resources::Material> material)
{
	if (m_materialsModel.size() == 0)
		m_materialsModel.push_back(material);
	else
		m_materialsModel[0] = material;
}

void Mesh::SetMainTexture(std::shared_ptr<Resources::Texture> texture, int material)
{
	if (m_materialsModel.size() <= material)
		return;

	if (m_materialsModel[material]->textures.size() == 0)
		m_materialsModel[material]->textures.push_back(texture);
	else
		m_materialsModel[material]->textures[0] = texture;
}
