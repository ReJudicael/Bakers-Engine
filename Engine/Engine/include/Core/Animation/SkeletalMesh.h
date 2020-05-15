#pragma once

#include "CoreMinimal.h"
#include "Mesh.h"
#include "Mat4.hpp"
#include "Bone.h"
#include "Animation.h"
#include "IUpdatable.hpp"

namespace Core::Animation
{
	struct BoneTMP
	{
		Core::Maths::Mat4	TRS;
		Bone				bone;
		BoneTMP*			parent;
	};

	BAKERS_API_CLASS SkeletalMesh : public Mesh, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		Bone m_rootBone;
		std::vector<BoneTMP> Bones;
		std::vector<Core::Maths::Mat4> m_finalTransforms;
		virtual void OnReset() override;
		virtual void OnCopy(IComponent* copyTo) const override;
	public:
		std::shared_ptr<Animation> m_testAnimation;
		float m_currTimeAnim{ 0.f };
		float m_speedAnim{ 1.f };
		bool canDraw{true};

		void UpdateBone(Bone& bone, Core::Maths::Mat4 parentTransform, std::shared_ptr<Animation> animation, const float& animationTime);
		void UpdateAllBones(std::shared_ptr<Animation> animation, const float& animationTime);

		Core::Maths::Mat4 BlendAnimation(std::shared_ptr<Animation> animation, Bone bone, BoneAnimation boneAnim, const float& animationTime);

		unsigned int Find(BoneAnimation animation, const float& animationTime);

		void initBones(std::shared_ptr<BoneTree> inBone);

		void initBonePos(Bone currBone, Core::Maths::Mat4 parent);

		/**
		 * Function inheritated from IRenderable,
		 * override for draw the mesh with the material and the model
		 * @param cam: the camera to render to
		 */
		virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

		virtual void OnUpdate(float deltaTime) override;

		virtual bool OnStart() override;
		virtual void OnInit() override;
		virtual void OnDestroy() override;

		virtual void AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial) override;

		REGISTER_CLASS(Mesh, Core::Datastructure::IUpdatable)
	};

}