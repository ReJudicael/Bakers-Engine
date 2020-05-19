#include "SkeletalMesh.h"
#include "Object.hpp"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "Model.h"
#include "Texture.h"
#include <string>

namespace Core::Animation
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<SkeletalMesh>("Skeletal Mesh")
			.constructor();
	}

	bool SkeletalMesh::OnStart()
	{
		if (IsModelLoaded())
		{
			//CreateAABBMesh();
			Mesh::OnStart();
			Core::Datastructure::IUpdatable::OnStart();
			return true;
		}
		return false;
	}

	void SkeletalMesh::OnInit()
	{
		Mesh::OnInit();
		Core::Datastructure::IUpdatable::OnInit();
	}

	void SkeletalMesh::OnDestroy()
	{
		Mesh::OnDestroy();
		Core::Datastructure::IUpdatable::OnDestroy();
	}

	void SkeletalMesh::OnReset()
	{
		Mesh::OnReset();
		Core::Datastructure::IUpdatable::OnReset();
	}

	void SkeletalMesh::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new SkeletalMesh();
		OnCopy(copyTo);
	}
	void SkeletalMesh::OnCopy(IComponent* copyTo) const
	{
		Mesh::OnCopy(copyTo);
		Core::Datastructure::IUpdatable::OnCopy(copyTo);
	}

	void	SkeletalMesh::UpdateModel()
	{
		Resources::Loader::ResourcesManager* manager{ GetRoot()->GetEngine()->GetResourcesManager() };
		
		if (m_isChild)
		{
			if (manager->GetCountSkeleton(m_modelName) <= 0)
				return;
			InitBones(manager->GetSkeleton(m_modelName));

			animationHandler = AnimationHandler(nullptr);
		}
		Mesh::UpdateModel();
	}

	void SkeletalMesh::InitBones(std::shared_ptr<BoneTree> inBone)
	{
		 m_rootBone = inBone->rootBone;
		 constexpr Core::Maths::Mat<4, 4> identity{ identity.Identity() };
		 m_finalTransforms.resize(inBone->numBone);

		 InitBonePos(m_rootBone, identity);
	}

	void SkeletalMesh::InitBonePos(std::shared_ptr<Bone> currBone, Core::Maths::Mat4 parent)
	{
		Core::Maths::Mat4 curr = parent * currBone->baseTransform.GetLocalTrs();
		m_finalTransforms[currBone->boneIndex] = curr * currBone->offsetBone;

		for (auto i = 0; i < currBone->child.size(); i++)
		{
			InitBonePos(currBone->child[i], curr);
		}
	}

	void SkeletalMesh::OnDraw(Core::Datastructure::ICamera* cam)
	{
		Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };

		while (!animationHandler.animationFinish)
		{
			trs = (m_parent->GetGlobalTRS());
		}

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
				// Send to the shader the transform matrix of bones
				for (auto j{ 0 }; j < m_finalTransforms.size(); j++)
				{
					std::string loc = "gBones[" + std::to_string(j) + "]";
					glUniformMatrix4fv(material.shader->GetLocation(loc), 1, GL_TRUE, m_finalTransforms[j].array);
				}
			}

			glDrawElements(GL_TRIANGLES, currOffsetMesh.count, GL_UNSIGNED_INT,
				(GLvoid*)(currOffsetMesh.beginIndices * sizeof(GLuint)));
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	void SkeletalMesh::OnUpdate(float deltaTime)
	{
		GetParent()->GetScene()->GetEngine()->GetResourcesManager()->
					m_task.AddTask(&Core::Animation::AnimationHandler::UpdateSkeletalMeshBones,
					&animationHandler, m_rootBone, std::ref(m_finalTransforms), deltaTime);
	}

	void SkeletalMesh::AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial)
	{
		if (!m_model)
		{
			m_materialsNames = namesMaterial;
		}
		else
		{
			for (int i{ 0 }; i < m_model->offsetsMesh.size(); i++)
			{
				m_materialsModel.push_back(resources.GetMaterial(namesMaterial[i]));
			}
		}
	}
}