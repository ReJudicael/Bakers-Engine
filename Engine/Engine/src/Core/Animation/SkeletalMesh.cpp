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
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
		registration::class_<SkeletalMesh>("Skeletal Mesh")
			.constructor();
	}

	bool SkeletalMesh::OnStart()
	{
		if (IsModelLoaded())
		{
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
		if (animationHandler)
		{
			delete animationHandler;
			animationHandler = nullptr;
		}
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

			//animationHandler = AnimationHandler(nullptr);
		}
		Mesh::UpdateModel();
	}

	void SkeletalMesh::InitBones(std::shared_ptr<BoneTree> inBone)
	{
		 m_rootBone = inBone->rootBone;
		 constexpr Core::Maths::Mat<4, 4> identity{ identity.Identity() };
		 m_finalTransforms.resize(inBone->numActiveBones);
		 m_numberBones = static_cast<GLuint>(inBone->numActiveBones);
		 m_finalBonesTRSfloat.resize(static_cast<size_t>(m_numberBones * static_cast <GLuint>(16)));

		 InitBonePos(m_rootBone, identity);
	}

	void SkeletalMesh::InitBonePos(std::shared_ptr<Bone> currBone, Core::Maths::Mat4 parent)
	{
		Core::Maths::Mat4 curr = parent * currBone->baseTransform.GetLocalTrs();
		if (currBone->activeBone)
		{
			Core::Maths::Mat4 finalTRS = curr * currBone->offsetBone;

			int currentPosInArrayFloat = currBone->boneIndex * 16;
			for (int i = 0; i < 16; i++)
			{
				m_finalBonesTRSfloat[currentPosInArrayFloat + i] = finalTRS.array[i];
			}
		}

		for (auto i = 0; i < currBone->child.size(); i++)
		{
			InitBonePos(currBone->child[i], curr);
		}
	}

	void SkeletalMesh::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		Core::Maths::Mat4 trs{ (m_parent->GetGlobalTRS()) };
		if(animationHandler)
			while (!animationHandler->animationFinish)
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
			FindMaterialToDelete(currOffsetMesh);

			int matIndice = currOffsetMesh.materialIndices;
			FindMaterialToDelete(currOffsetMesh);

			std::shared_ptr<Resources::Material> material = m_materialsModel[matIndice];
			std::shared_ptr<Resources::Shader> usedShader;
			if (material->IsSkeletal)
			{
				usedShader = (givenShader ? GetRoot()->GetEngine()->GetResourcesManager()->GetShader("SkeletalShadow") 
								: material->shader);
			}
			else
			{
				usedShader = (givenShader ? givenShader : material->shader);
			}
			usedShader->UseProgram();
			{
				MaterialSendToOpengGL(view, proj, trs.array, material, usedShader);

				if (material->IsSkeletal)
				{
					// Send to the shader the transform matrix of bones
					glUniformMatrix4fv(	usedShader->GetLocation("uBones"), m_numberBones, GL_TRUE, 
										static_cast<GLfloat*>(m_finalBonesTRSfloat.data()));
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
		if(animationHandler)
			GetParent()->GetScene()->GetEngine()->GetResourcesManager()->
						m_task->AddTask(&Core::Animation::AnimationHandler::UpdateSkeletalMeshBones,
						animationHandler, m_rootBone, std::ref(m_finalBonesTRSfloat), deltaTime);
	}
}