#include "AnimationHandler.h"


namespace Core::Animation
{
	Core::Maths::Quat Nlerp(const Core::Maths::Quat& Q1, const Core::Maths::Quat& Q2, const float& t)
	{
		float bias = Q1.Dot(Q2) >= 0 ? 1 : -1;
		return (Q1 * (1 - t) * bias + Q2 * t).Normalized();
	}


	void TransitionNode::InitTransition(std::shared_ptr<AnimationNode> currentAnimationNode,
		std::shared_ptr<AnimationNode> nextAnimationNode, std::function<bool()> conditionTransition)
	{
		if (!currentAnimationNode || !nextAnimationNode)
			return;

		m_currentAnimationNode = currentAnimationNode;
		m_nextAnimationNode = nextAnimationNode;

		// if no conditionTransition isn't give 
		// create a default transition
		if (conditionTransition)
			m_conditionTransition = conditionTransition;
		else
			m_conditionTransition = std::bind(&AnimationNode::DefaultConditionAnimationNode, m_currentAnimationNode);
	}

	Core::Maths::Mat4 TransitionNode::TransitionAnimation(std::shared_ptr<Bone> bone)
	{
		// calculate the animation blend of the currentAnimation
		Core::Datastructure::Transform TRS1;
		m_currentAnimationNode->BlendAnimation(bone, TRS1);

		// calculate the animation blend of the nextAnimation
		Core::Datastructure::Transform TRS2;
		m_nextAnimationNode->BlendAnimation(bone, TRS2);

		float weight = m_currentTime;

		Core::Maths::Vec3 position{ (TRS1.GetLocalPos()) * (1 - weight) + (TRS2.GetLocalPos()) * weight };

		Core::Maths::Quat rotation{ Nlerp(TRS1.GetLocalRot(), TRS2.GetLocalRot(), weight) };

		return Core::Datastructure::Transform{ position, rotation, TRS1.GetLocalScale() }.GetLocalTrs();
	}

	Core::Maths::Mat4 TransitionNode::UpdateBoneTransition(std::shared_ptr<Bone> bone)
	{
		if (m_currentAnimationNode->nodeAnimation->animationTree.count(bone->boneName) > 0 
			&& m_nextAnimationNode->nodeAnimation->animationTree.count(bone->boneName) > 0)
		{
			return TransitionAnimation(bone);
		}
	}

	void TransitionNode::UpdateTimer(float deltaTime)
	{
		m_currentTime += deltaTime * speed;

		if (m_currentTime > 1.f)
		{
			m_currentTime = 0.f;
			isFinish = true;
		}
	}

	void TransitionNode::Reset()
	{
		m_currentTime = 0.f;
		isFinish = false;
	}

	Core::Maths::Mat4 AnimationNode::BlendAnimation(std::shared_ptr<Bone> bone, Core::Datastructure::Transform& out)
	{

		std::shared_ptr<BoneAnimation> boneAnim = nodeAnimation->animationTree[bone->boneName];

		// if the bone have juste one key in the animation no need to blend 
		if (boneAnim->timeKey.size() == 1)
			return Core::Datastructure::Transform{ boneAnim->boneLocalTransforms[0].localPosition,
													boneAnim->boneLocalTransforms[0].localRotation,
													boneAnim->boneLocalTransforms[0].localScale }.GetLocalTrs();


		unsigned int curr = FindBoneAnimationIndex(boneAnim);
		unsigned int next = curr + 1;
		if (next >= boneAnim->boneLocalTransforms.size())
			next = curr;

		BoneLocalTransform currLocal = boneAnim->boneLocalTransforms[curr];
		BoneLocalTransform nextLocal = boneAnim->boneLocalTransforms[next];

		float weight = (m_currentTime - boneAnim->timeKey[curr]) / (boneAnim->timeKey[next] - boneAnim->timeKey[curr]);


		Core::Maths::Vec3 position{ (currLocal.localPosition) * (1 - weight) + (nextLocal.localPosition) * weight };

		Core::Maths::Quat rotation{ Nlerp(currLocal.localRotation,nextLocal.localRotation, weight) };

		out = Core::Datastructure::Transform{ position, rotation, currLocal.localScale };
		return out.GetLocalTrs();
	}


	unsigned int AnimationNode::FindBoneAnimationIndex(std::shared_ptr<BoneAnimation> boneAnim)
	{
		unsigned int index;
		for (index = 0; index < boneAnim->timeKey.size() - 1; index++)
		{
			if (m_currentTime < boneAnim->timeKey[index + 1])
				return index;
		}
		return index;
	}

	void AnimationNode::UpdateAnimationBone(std::shared_ptr<Bone> currentBone, Core::Maths::Mat4 parent, 
											std::vector<Core::Maths::Mat4>& finalTransform)
	{
		Core::Maths::Mat4 nodeTransform{ currentBone->baseTransform.GetLocalTrs() };

		if (!m_inTransition)
		{
			if (nodeAnimation->animationTree.count(currentBone->boneName) > 0)
			{
				Core::Datastructure::Transform TRS;
				nodeTransform = BlendAnimation(currentBone, TRS);
			}
		}
		else
		{
			nodeTransform = transitionsAnimation[indexTransition]->UpdateBoneTransition(currentBone);
		}

		Core::Maths::Mat4 globalTransform = parent * nodeTransform;

		finalTransform[currentBone->boneIndex] = (globalTransform * (currentBone->offsetBone));

		for (auto i{ 0 }; i < currentBone->child.size(); i++)
		{
			UpdateAnimationBone(currentBone->child[i], globalTransform, finalTransform);
		}
	}

	void AnimationNode::UpdateAnimationNode(float deltaTime)
	{
		UpdateTimer(deltaTime);

		if (m_inTransition)
		{
			transitionsAnimation[indexTransition]->UpdateTimer(deltaTime);
			//if (transitionsAnimation[indexTransition]->m_nextAnimationNode)
				//transitionsAnimation[indexTransition]->m_nextAnimationNode->UpdateTimer(deltaTime);
			if (!transitionsAnimation[indexTransition]->m_nextAnimationNode)
				transitionsAnimation[indexTransition]->Reset();
		}
		else
		{
			for (auto i{ 0 }; i < transitionsAnimation.size(); i++)
			{
				if (transitionsAnimation[i] && transitionsAnimation[i]->m_conditionTransition())
				{
					m_inTransition = true;
					indexTransition = i;
					break;
				}
			}
		}
	}

	void AnimationNode::UpdateTimer(float deltaTime)
	{
		m_currentTime += deltaTime * speed;

		if (m_currentTime > nodeAnimation->Time)
		{
			if (Loop)
			{
				reset = true;
				m_currentTime = 0.f;
			}
			else
			{
				m_currentTime = nodeAnimation->Time;
			}
		}
	}

	bool AnimationNode::DefaultConditionAnimationNode()
	{
		if (m_currentTime >= nodeAnimation->Time || reset)
			return true;
		else
			return false;
	}

	void AnimationNode::SetNextAnimation(std::shared_ptr<AnimationNode>& currentAnimation)
	{
		//reset the currentAnimation and his transition
		reset = false;
		m_inTransition = false;
		m_currentTime = 0.f;
		transitionsAnimation[indexTransition]->Reset();
		//change the currentAnimation to the nextAnimation
		currentAnimation = transitionsAnimation[indexTransition]->m_nextAnimationNode;
		indexTransition = 0;
	}

	AnimationHandler::AnimationHandler(std::shared_ptr<AnimationNode> animationNode):
		m_currentAnimationNode{ animationNode }
	{}

	void AnimationHandler::UpdateSkeletalMeshBones(std::shared_ptr<Bone> rootBone, 
													std::vector<Core::Maths::Mat4>& finalTransform, float deltaTime)
	{
		if (m_currentAnimationNode)
		{
			m_currentAnimationNode->UpdateAnimationNode(deltaTime);

			constexpr Core::Maths::Mat<4, 4> identity{ identity.Identity() };

			for (auto i{ 0 }; i < rootBone->child.size(); i++)
			{
				m_currentAnimationNode->UpdateAnimationBone(rootBone->child[i], identity, finalTransform);
			}

			// check if the Transition of the currentAniation is finish
			if (m_currentAnimationNode->IsTransitionFinish())
			{
				m_currentAnimationNode->SetNextAnimation(m_currentAnimationNode);
			}
		}

		animationFinish = true;
	}

	void AnimationHandler::PlayAnimation(std::shared_ptr<AnimationNode> animation)
	{
		m_currentAnimationNode = animation;
	}

}