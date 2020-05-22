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
	ZoneScopedN("Registering RTTR")
	registration::class_<Mesh>("Mesh")
		.constructor()
		.property("Model", &Mesh::GetModel, &Mesh::SetModel)
		.property("Materials", &Mesh::GetMaterials, &Mesh::SetMaterials)
		.property("IsRoot", &Mesh::m_isRoot, detail::protected_access())
		.property("MaterialsNames", &Mesh::m_materialsNames, detail::protected_access())
		.property("IsChild", &Mesh::m_isChild, detail::protected_access())
		.property("Cast Shadows", &Mesh::m_castShadow);
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

void Mesh::SetMaterials(std::vector<std::string> newMaterials)
{
	for (auto i = 0; i < m_materialsNames.size(); i++)
	{
		if (newMaterials[i] != m_materialsNames[i])
		{
			if (ChangeOneMaterial(newMaterials[i], i))
			{
				m_materialsNames = newMaterials;
				break;
			}
		}
	}
}

bool Mesh::ChangeOneMaterial(std::string newMaterial, int iD)
{
	if (iD >= m_materialsModel.size())
		return false;

	Resources::Loader::ResourcesManager* manager{ GetRoot()->GetEngine()->GetResourcesManager() };
	if (manager->GetCountMaterials(newMaterial))
	{
		m_materialsModel[iD] = manager->GetMaterial(newMaterial);
		return true;
	}

	//load the JSON
	//if (newMaterials[iD].find(".BakersMaterial"))
		//load if not

	return false;
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
	ZoneScoped
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

void Mesh::CreateAABBMesh(physx::PxScene*& scene)
{
	Core::Datastructure::Object* object = GetParent();
	
	Core::Datastructure::IComponent* component = dynamic_cast<Core::Datastructure::IComponent*>(this);
	physx::PxRigidActor* actor = object->GetScene()
							->GetEngine()
							->GetPhysicsScene()
							->CreateEditorPhysicsActor(static_cast<void*>(component), object->GetUpdatedTransform(), m_model, scene);
	
	m_destroyActorEvent += std::bind(&Core::Physics::PhysicsScene::DestroyEditorPhysicActor , object->GetScene()
		->GetEngine()->GetPhysicsScene(), actor, scene);

	GetParent()->SetAnEventTransformChange(std::bind(&Core::Physics::PhysicsScene::UpdatePoseOfActor, object->GetScene()
											->GetEngine()->GetPhysicsScene(), actor));
}

void Mesh::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
{
	ZoneScoped
	// If the draw is made for shadow mapping and the mesh cannot cast shadows
	if (givenShader && !m_castShadow)
		return;

	Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };

	// check if the mesh have a modelMesh
	if (m_model == nullptr)
		return;
	// check if the VAO of the model is link to OpenGL
	if (m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
		return;

	glEnable(GL_DEPTH_TEST);

	Display(view, proj, trs.array, givenShader);
}

void Mesh::Display(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, float* trs, std::shared_ptr<Resources::Shader> givenShader)
{
	glBindVertexArray(m_model->VAOModel);

	for (int i = 0; i < m_model->offsetsMesh.size(); i++)
	{
		Resources::OffsetMesh currOffsetMesh = m_model->offsetsMesh[i];

		Resources::Material material = *m_materialsModel[currOffsetMesh.materialIndices];

		std::shared_ptr<Resources::Shader> usedShader = (givenShader ? givenShader : material.shader);

		usedShader->UseProgram();
		{
			// Init value of texture1 for mesh texture
			glUniform1i(usedShader->GetLocation("uColorTexture"), 0);
			// Init value of texture2 for normal map
			glUniform1i(usedShader->GetLocation("uNormalMap"), 1);
			// Init value of higher texture for shadow maps
			for (size_t i{0}; i < 10; ++i)
				glUniform1i(usedShader->GetLocation("uShadowMap[" + std::to_string(i) + "]"), 2 + i);

			material.SendMaterial();
			glUniformMatrix4fv(usedShader->GetLocation("uModel"), 1, GL_TRUE, trs);
			glUniformMatrix4fv(usedShader->GetLocation("uCam"), 1, GL_TRUE, view.array);
			glUniformMatrix4fv(usedShader->GetLocation("uProj"), 1, GL_FALSE, proj.array);
			
			std::vector<Core::Renderer::Light*> lights = Resources::Shader::GetShadowCastingLights();
			for (size_t i{ 0 }; i < lights.size(); ++i)
				glUniformMatrix4fv(usedShader->GetLocation("uLightView[" + std::to_string(i) + "]"), 1, GL_TRUE, lights[i]->GetViewFromLight().array);
			glUniform1i(usedShader->GetLocation("uShadowCount"), lights.size());
		}

		glDrawElements(GL_TRIANGLES, currOffsetMesh.count, GL_UNSIGNED_INT,
			(GLvoid*)(currOffsetMesh.beginIndices * sizeof(GLuint)));
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Mesh::AddToNavMesh()
{
	if (m_parent->GetFlags().test_flag(Core::Datastructure::ObjectFlags::STATIC) && m_model)
		GetRoot()->GetEngine()->AddMeshToNav(m_model->vertices.data(), static_cast<int>(m_model->vertices.size()), m_model->indices.data(), static_cast<int>(m_model->indices.size()), GetParent()->GetUpdatedTransform());
}

void Mesh::DrawFixedMesh(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, Core::Maths::Mat4 trs)
{
	// check if the mesh have a modelMesh
	if (m_model == nullptr)
		return;
	// check if the VAO of the model is link to OpenGL
	if (m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
		return;

	Display(view, proj, trs.array);
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
		m_materialsNames = namesMaterial;
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
	ZoneScoped
	ComponentBase::OnCopy(copyTo);
	IRenderable::OnCopy(copyTo);
}

void Mesh::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
	copyTo = new Mesh();
	OnCopy(copyTo);
}

bool Mesh::OnStart()
{
	ZoneScoped
	if (IsModelLoaded())
	{
		//CreateAABBMesh();
		/*Core::Datastructure::Object* object = GetParent();
		GetRoot()->GetEngine()->AddMeshToNav(m_model->vertices.data(), static_cast<int>(m_model->vertices.size()),
												m_model->indices.data(), static_cast<int>(m_model->indices.size()), 
												object->GetUpdatedTransform());*/
		GetRoot()->GetEngine()->InitMesh(this);

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
