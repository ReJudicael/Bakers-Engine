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
	/*struct ObjectBone
	{
		Core::Datastructure::Transform				globalPosition;
		std::vector<ObjectBone>						childBone;
		std::vector<Core::Datastructure::Object*>	childObject;
	};*/

	BAKERS_API_CLASS SkeletalMesh : public Mesh, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		std::shared_ptr<Bone>			m_rootBone;
		//std::shared_ptr<ObjectBone>		treeBone;
		std::vector<Core::Maths::Mat4> m_finalTransforms;

	public:

		AnimationHandler animationHandler;

	protected:
		virtual bool OnStart() override;				
		virtual void	OnReset() override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	StartCopy(IComponent*& copyTo) const override;

		virtual void	UpdateModel() override;

	public:
		SkeletalMesh() = default;

		void InitBones(std::shared_ptr<BoneTree> inBone);

		void InitBonePos(std::shared_ptr<Bone> currBone, Core::Maths::Mat4 parent);

		/**
		 * Function inheritated from IRenderable,
		 * override for draw the mesh with the material and the model
		 * @param cam: the camera to render to
		 */
		virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

		virtual void OnUpdate(float deltaTime) override;
														
		virtual void OnInit() override;
		virtual void OnDestroy() override;

		virtual void AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial) override;

		REGISTER_CLASS(Mesh, Core::Datastructure::IUpdatable)
	};

}