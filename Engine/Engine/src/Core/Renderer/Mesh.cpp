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
	registration::class_<Core::Renderer::Mesh>("Mesh")
		.constructor()
		.property("Model", &Core::Renderer::Mesh::GetModel, &Core::Renderer::Mesh::SetModel)
		.property("Materials", &Core::Renderer::Mesh::GetMaterials, &Core::Renderer::Mesh::SetMaterials)
		.property("IsRoot", &Core::Renderer::Mesh::m_isRoot, detail::protected_access())
		.property("MaterialsNames", &Core::Renderer::Mesh::m_materialsNames, detail::protected_access())
		.property("IsChild", &Core::Renderer::Mesh::m_isChild, detail::protected_access())
		.property("Cast Shadows", &Core::Renderer::Mesh::m_castShadow);
}

void Core::Renderer::Mesh::SetModel(std::string newModel)
{
	if (m_modelName != "")
	{
		m_isRoot = false;
		m_isChild = false;
	}
	m_modelName = newModel;
	if (!IsInit())
		return;
	
	m_materialsModel.clear();
	m_materialsNames.clear();
	UpdateModel();
}

void Core::Renderer::Mesh::SetMaterials(std::vector<std::string> newMaterials)
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

bool Core::Renderer::Mesh::ChangeOneMaterial(std::string newMaterial, int iD)
{
	if (iD >= m_materialsModel.size())
		return false;

	Resources::Loader::ResourcesManager* manager{ GetRoot()->GetEngine()->GetResourcesManager() };
	if (manager->GetCountMaterials(newMaterial))
	{
		m_materialsModel[iD] = manager->GetMaterial(newMaterial);
		// delete the event in the material for change the name
		// create new event for rename the material
		int index = m_materialsModel[iD]->UpdateNameMaterial.AddListener(
			std::bind(&Core::Renderer::Mesh::UpdateNameMaterial, this, iD, std::placeholders::_1));
		if (iD < m_destroyMaterialEvent.size())
		{
			m_destroyMaterialEvent[iD]();
		}
		// change the event who destroy the event of the material
		m_destroyMaterialEvent[iD] = std::bind(&Core::Renderer::Mesh::DeleteMaterialEvent, this, iD, index);
		return true;
	}

	if (newMaterial.find(".bmat"))	
	{ 
		if (manager->GetMaterial(newMaterial))
		{
			m_materialsModel[iD] = manager->GetMaterial(newMaterial);
			// delete the event in the material for change the name
			m_destroyMaterialEvent[iD]();
			// create new event for rename the material
			int index = m_materialsModel[iD]->UpdateNameMaterial.AddListener(
				std::bind(&Core::Renderer::Mesh::UpdateNameMaterial, this, iD, std::placeholders::_1));
			// change the event who destroy the event of the material
			m_destroyMaterialEvent[iD] = std::bind(&Core::Renderer::Mesh::DeleteMaterialEvent, this, iD, index);
			return true;
		}
		return false;
	}
	return false;
}

void Core::Renderer::Mesh::SetChildModel(std::string newModel)
{
	m_isRoot = false;
	m_isChild = true;
	m_modelName = newModel;
	if (!IsInit())
		return;
	UpdateModel();
}

void Core::Renderer::Mesh::UpdateModel()
{
	if (m_modelName == "")
		return;
	
	Resources::Loader::ResourcesManager* manager{ GetRoot()->GetEngine()->GetResourcesManager() };
	GetRoot()->GetEngine()->LoadObject(m_modelName.c_str());

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
			if (m_materialsNames.size() > 0)
				AddMaterials(*manager, m_materialsNames);
			else
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
	m_destroyActorEvent.Invoke();
	GetRoot()->GetEngine()->InitMesh(this);
}

Core::Renderer::Mesh::Mesh() : ComponentBase(), m_projection{ nullptr }
{
	ZoneScoped
}

Core::Renderer::Mesh::~Mesh()
{
	glDeleteTextures(1, &m_texture);
	//glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_VAO);
	//glDeleteProgram(m_program);
}

void Core::Renderer::Mesh::OnInit()
{
	IRenderable::OnInit();
	UpdateModel();
}

void Core::Renderer::Mesh::OnDestroy()
{
	for (size_t i{ 0 }; i < m_destroyMaterialEvent.size(); i++)
		m_destroyMaterialEvent[i]();
	m_destroyActorEvent.Invoke();
	IRenderable::OnDestroy();
}

void Core::Renderer::Mesh::SendProjectionMatrix(Core::Maths::Mat4 data)
{
	m_projection = data.array;
}

bool Core::Renderer::Mesh::IsModelLoaded()
{
	return m_model && m_model->stateVAO == Resources::EOpenGLLinkState::ISLINK;
}

bool Core::Renderer::Mesh::CreateAABBMesh(physx::PxScene*& scene)
{
	if (!m_model || m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
		return false;
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

	return true;
}

void Core::Renderer::Mesh::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
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

void  Core::Renderer::Mesh::Display(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, float* trs, std::shared_ptr<Resources::Shader> givenShader)
{
	glBindVertexArray(m_model->VAOModel);

	for (auto i{ 0 }; i < m_model->offsetsMesh.size(); i++)
	{
		Resources::OffsetMesh currOffsetMesh = m_model->offsetsMesh[i];
		FindMaterialToDelete(currOffsetMesh);
		std::shared_ptr<Resources::Material> material = m_materialsModel[currOffsetMesh.materialIndices];
		std::shared_ptr<Resources::Shader> usedShader = (givenShader ? givenShader : material->shader);

		usedShader->UseProgram();
		MaterialSendToOpengGL(view, proj, trs, material, usedShader);

		glDrawElements(GL_TRIANGLES, currOffsetMesh.count, GL_UNSIGNED_INT,
			(GLvoid*)(currOffsetMesh.beginIndices * sizeof(GLuint)));
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Core::Renderer::Mesh::FindMaterialToDelete(const Resources::OffsetMesh& currOffsetMesh)
{
	if (m_materialsModel[currOffsetMesh.materialIndices]->IsDelete)
	{
		m_materialsModel[currOffsetMesh.materialIndices] = GetRoot()->GetEngine()->GetResourcesManager()->GetMaterial("Default");
		m_materialsNames[currOffsetMesh.materialIndices] = "Default";
	}
}

void Core::Renderer::Mesh::MaterialSendToOpengGL(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, float* trs, 
													std::shared_ptr<Resources::Material> mat, std::shared_ptr<Resources::Shader> shader)
{
	// Init value of texture1 for mesh texture
	glUniform1i(shader->GetLocation("uColorTexture"), 0);
	// Init value of texture2 for normal map
	glUniform1i(shader->GetLocation("uNormalMap"), 1);
	mat->SendMaterial();

	glUniformMatrix4fv(shader->GetLocation("uModel"), 1, GL_TRUE, trs);
	glUniformMatrix4fv(shader->GetLocation("uCam"), 1, GL_TRUE, view.array);
	glUniformMatrix4fv(shader->GetLocation("uProj"), 1, GL_FALSE, proj.array);

}

void  Core::Renderer::Mesh::AddToNavMesh()
{
	if (m_parent->GetFlags().test_flag(Core::Datastructure::ObjectFlags::STATIC) && m_model)
		GetRoot()->GetEngine()->AddMeshToNav(m_model->vertices.data(), static_cast<int>(m_model->vertices.size()), m_model->indices.data(), static_cast<int>(m_model->indices.size()), GetParent()->GetUpdatedTransform());
}

void  Core::Renderer::Mesh::DrawFixedMesh(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, Core::Maths::Mat4 trs)
{
	// check if the mesh have a modelMesh
	if (m_model == nullptr)
		return;
	// check if the VAO of the model is link to OpenGL
	if (m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
		return;

	Display(view, proj, trs.array);
}

void  Core::Renderer::Mesh::AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial)
{
	if (!m_model)
	{
		m_materialsNames = namesMaterial;
	}
	else
	{
		m_materialsNames = namesMaterial;
		m_destroyMaterialEvent.clear();
		for (int i{ 0 }; i < m_model->offsetsMesh.size(); i++)
		{
			m_materialsModel.push_back(resources.GetMaterial(namesMaterial[i]));
			if (m_materialsModel[i])
			{
				int index = m_materialsModel[i]->UpdateNameMaterial.AddListener(
								std::bind(&Core::Renderer::Mesh::UpdateNameMaterial, this, i, std::placeholders::_1));
				m_destroyMaterialEvent.push_back(std::bind(&Core::Renderer::Mesh::DeleteMaterialEvent, this, i, index));
			}
			else
			{
				m_materialsModel[i] = resources.GetMaterial("Default");
				m_materialsNames[i] = "Default";
				m_destroyMaterialEvent.push_back(std::bind(&Core::Renderer::Mesh::DeleteMaterialEvent, this, i, 0));
			}
		}
	}
}

int  Core::Renderer::Mesh::GetVertexCount()
{
	int size = 0;
	for (int i = 0; i < m_model->offsetsMesh.size(); ++i)
		size += m_model->offsetsMesh[i].count;
	return size;
}

void Core::Renderer::Mesh::OnReset()
{
	ComponentBase::OnReset();
	IRenderable::OnReset();
}

void  Core::Renderer::Mesh::OnCopy(IComponent* copyTo) const
{
	ZoneScoped
	ComponentBase::OnCopy(copyTo);
	IRenderable::OnCopy(copyTo);
}

void  Core::Renderer::Mesh::StartCopy(IComponent*& copyTo) const
{
	ZoneScoped
	copyTo = new  Core::Renderer::Mesh();
	OnCopy(copyTo);
}

bool  Core::Renderer::Mesh::OnStart()
{
	ZoneScoped
	if (IsModelLoaded())
	{
		Core::Datastructure::Object* object = GetParent();
		GetRoot()->GetEngine()->AddMeshToNav(m_model->vertices.data(), static_cast<int>(m_model->vertices.size()),
												m_model->indices.data(), static_cast<int>(m_model->indices.size()), 
												object->GetUpdatedTransform());

		IRenderable::OnStart();
		ComponentBase::OnStart();
		return true;
	}
	return false;
}

std::shared_ptr<Resources::Material>  Core::Renderer::Mesh::GetMainMaterial()
{
	if (m_materialsModel.size() == 0)
		return nullptr;
	else
		return m_materialsModel[0];
}

void  Core::Renderer::Mesh::SetMainMaterial(std::shared_ptr<Resources::Material> material)
{
	if (m_materialsModel.size() == 0)
		m_materialsModel.push_back(material);
	else
		m_materialsModel[0] = material;
}

void  Core::Renderer::Mesh::SetMainTexture(std::shared_ptr<Resources::Texture> texture, int material)
{
	if (m_materialsModel.size() <= material)
		return;

	if (m_materialsModel[material]->textures.size() == 0)
		m_materialsModel[material]->textures.push_back(texture);
	else
		m_materialsModel[material]->textures[0] = texture;
}
