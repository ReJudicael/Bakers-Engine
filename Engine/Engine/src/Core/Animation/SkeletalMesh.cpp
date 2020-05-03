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
		registration::class_<SkeletalMesh>("SkeletalMesh")
			.constructor()
			.property("vertex count", &SkeletalMesh::m_speedAnim);
	}

	void SkeletalMesh::OnStart()
	{
		Mesh::OnStart();
		Core::Datastructure::IUpdatable::OnStart();
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

	void SkeletalMesh::OnCopy(IComponent* copyTo) const
	{
		Mesh::OnCopy(copyTo);
		Core::Datastructure::IUpdatable::OnCopy(copyTo);
	}

	Core::Maths::Quat Nlerp(const Core::Maths::Quat& Q1, const Core::Maths::Quat& Q2, const float& t)
	{
		float bias = Q1.Dot(Q2) >= 0 ? 1 : -1;
		return (Q1 * (1 - t) * bias + Q2 * t).Normalized();
	}

	void SkeletalMesh::UpdateBone(Bone& bone, Core::Maths::Mat4 parentTransform, std::shared_ptr<Animation> animation, const float& animationTime)
	{
		Core::Maths::Mat4 nodeTransform{ bone.baseTransform.GetLocalTrs() };

		if (animation->animationTree.count(bone.boneName) > 0)
		{
			// TODO calculate the position of the animation
			nodeTransform = BlendAnimation(animation, bone, animation->animationTree[bone.boneName], animationTime);
		}
		
		Core::Maths::Mat4 globalTransform = parentTransform * nodeTransform;
		
		m_finalTransforms[bone.boneIndex] = (/*m_InverseGlobal.Inversed() **/ globalTransform * ( bone.offsetBone));

		for (auto i{ 0 }; i < bone.child.size(); i++)
		{
			UpdateBone(bone.child[i], globalTransform, animation, animationTime);
		}
	}

	Core::Maths::Mat4 SkeletalMesh::BlendAnimation(std::shared_ptr<Animation> animation, Bone bone, BoneAnimation boneAnim, const float& animationTime)
	{
		if (boneAnim.timeKey.size() == 1)
			return Core::Datastructure::Transform{ boneAnim.boneLocalTransforms[0].localPosition, 
													boneAnim.boneLocalTransforms[0].localRotation, 
													boneAnim.boneLocalTransforms[0].localScale }.GetLocalTrs();


		unsigned int curr = Find(boneAnim, animationTime);
		unsigned int next = curr + 1;
		//std::cout << "curr" << curr << std::endl;
		//std::cout << "next" << next << std::endl;
		BoneLocalTransform currLocal = boneAnim.boneLocalTransforms[curr];
		BoneLocalTransform nextLocal = boneAnim.boneLocalTransforms[next];

		float weight = (animationTime - boneAnim.timeKey[curr]) / (boneAnim.timeKey[next] - boneAnim.timeKey[curr]);


		//Core::Maths::Vec3 position{ (bone.baseTransform.GetGlobalPos() + currLocal.localPosition) * (1 - weight) + (bone.baseTransform.GetGlobalPos() + nextLocal.localPosition) * weight };
		Core::Maths::Vec3 position{ (currLocal.localPosition) * (1 - weight) + (nextLocal.localPosition) * weight };
		
		//Core::Maths::Vec3 scale{ currLocal.localScale * (1 - weight) + nextLocal.localScale * weight };
		//Core::Maths::Quat rotation{ Nlerp(bone.baseTransform.GetGlobalRot() * currLocal.localRotation, bone.baseTransform.GetGlobalRot() * nextLocal.localRotation, weight )};
		Core::Maths::Quat rotation{ Nlerp(currLocal.localRotation,nextLocal.localRotation, weight )};
		
		//rotation.Normalize();

		return Core::Datastructure::Transform{ position, rotation, currLocal.localScale }.GetLocalTrs();
	}

	unsigned int SkeletalMesh::Find(BoneAnimation boneAnim,const float& animationTime)
	{
		for (auto i{ 0 }; i < boneAnim.timeKey.size() - 1; i++)
		{
			if (animationTime < boneAnim.timeKey[i + 1])
				return i;
		}
	}

	void SkeletalMesh::initBone(std::shared_ptr<BoneTree> inBone)
	{
		 m_rootBone = inBone->rootBone;
		 m_finalTransforms.resize(inBone->numBone);
		 m_InverseGlobal = inBone->inverseGlobal;
	}

	void SkeletalMesh::OnDraw(Core::Datastructure::ICamera* cam)
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

		for (int i = 0; i < m_model->offsetsMesh.size() /*1*/; i++)
		{
			//Resources::OffsetMesh currOffsetMesh = m_model->offsetsMesh[2];
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
				// Send to the shader the transform matrix of bones
				for (auto j{ 0 }; j < m_finalTransforms.size(); j++)
				{
					std::string loc = "gBones[" + std::to_string(j) + "]";
					glUniformMatrix4fv(material.shader->GetLocation(loc), 1, GL_TRUE, m_finalTransforms[j].m_array);
				}
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

	void SkeletalMesh::OnUpdate(float deltaTime)
	{
		m_currTimeAnim += deltaTime * m_speedAnim;
		if (m_currTimeAnim > m_testAnimation->Time)
			m_currTimeAnim = 0.f;
		constexpr Core::Maths::Mat<4, 4> identity{ identity.Identity() };

		for (auto i{ 0 }; i < m_rootBone.child.size(); i++)
		{
			UpdateBone(m_rootBone.child[i], /*identity*/m_rootBone.baseTransform.GetLocalTrs(), m_testAnimation, m_currTimeAnim);
		}
	}

	void SkeletalMesh::AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial)
	{
		for (int i{ 0 }; i < m_model->offsetsMesh.size(); i++)
		{
			if (i < namesMaterial.size())
				m_materialsModel.push_back(resources.GetMaterial(namesMaterial[i]));
			else
				m_materialsModel.push_back(resources.GetMaterial(namesMaterial[0]));

			m_materialsModel[i]->shader = resources.GetShader("Skeletal");
		}
	}
}