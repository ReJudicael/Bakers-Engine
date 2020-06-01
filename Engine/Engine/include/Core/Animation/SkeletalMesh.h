#pragma once

#include "CoreMinimal.h"
#include "Mesh.h"
#include "Mat4.hpp"
#include "Bone.h"
#include "Animation.h"
#include "IUpdatable.hpp"
#include "AnimationHandler.h"

namespace Core::Animation
{

	BAKERS_API_CLASS SkeletalMesh : public Core::Renderer::Mesh, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		std::shared_ptr<Bone>			m_rootBone { nullptr };
		GLuint							m_numberBones{0};
		std::vector<Core::Maths::Mat4>	m_finalTransforms;
		std::vector<float>				m_finalBonesTRSfloat;

	public:

		AnimationHandler* animationHandler{ nullptr };

	protected:
		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for start the Mesh, and the IUpdatable
		 */
		virtual bool	OnStart() override;		
		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for create the copyTo as a SkeletalMesh
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;
		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for copy the SkeletalMesh
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;
		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for reset the Mesh, and the IUpdatable
		 */
		virtual void	OnReset() override;
		/**
		 * Function inheritated from Mesh,
		 * override for update the model and the rootbone
		 */
		virtual void	UpdateModel() override;

	public:
		SkeletalMesh() = default;

		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for init the Mesh, and the IUpdatable
		 */
		virtual void OnInit() override;

		/**  
		 * Init the root bone of the SkeletalMesh
		 * @param inBone: the BoneTree for init the SkeletalMesh
		 */
		void InitBones(std::shared_ptr<BoneTree> inBone);

		/**
		 * Init the bone position for draw them
		 * @param currBone: the current bone 
		 * @param parent: the parent matrix
		 */
		void InitBonePos(std::shared_ptr<Bone> currBone, Core::Maths::Mat4 parent);

		/**
		 * Function inheritated from Mesh,
		 * override for draw the mesh with the material and the model
		 * @param cam: the camera to render to
		 */
		virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr) override;

		/**
		 * Function inheritated from IUpdatable,
		 * override for update the AnimationHandler
		 * @param deltaTime: the delta time
		 */
		virtual void OnUpdate(float deltaTime) override;
		/**
		 * Function inheritated from Mesh and IUpdatable,
		 * override for destroy the Mesh, the IUpdatable
		 */
		virtual void OnDestroy() override;

		REGISTER_CLASS(Core::Renderer::Mesh, Core::Datastructure::IUpdatable)
	};

}