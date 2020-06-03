#include "AnimationHandler.h"


namespace Core::Animation
{
	Core::Maths::Quat Nlerp(const Core::Maths::Quat& Q1, const Core::Maths::Quat& Q2, const float& t)
	{
		float bias = Q1.Dot(Q2) >= 0 ? 1.f : -1.f;
		return (Q1 * (1 - t) * bias + Q2 * t).Normalized();
	}

	TransitionNode::TransitionNode(AnimationNode* currentAnimationNode,
		AnimationNode* nextAnimationNode, float speedTrans,
		std::function<bool()> conditionTransition):
		speed{ speedTrans }
	{
		InitTransition(currentAnimationNode, nextAnimationNode, conditionTransition);
	}

	void TransitionNode::InitTransition(AnimationNode* currentAnimationNode,
										AnimationNode* nextAnimationNode, std::function<bool()> conditionTransition)
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

	Core::Maths::Mat4 TransitionNode::TransitionAnimation(const std::shared_ptr<Bone>& bone)
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

	Core::Maths::Mat4 TransitionNode::UpdateBoneTransition(const std::shared_ptr<Bone>& bone)
	{
		if (m_currentAnimationNode->nodeAnimation->animationTree.count(bone->boneName) > 0 
			&& m_nextAnimationNode->nodeAnimation->animationTree.count(bone->boneName) > 0)
		{
			return TransitionAnimation(bone);
		}
		return bone->baseTransform.GetLocalTrs();
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

	bool TransitionNode::IsTransitionFinish()
	{
		return isFinish;
	}

	void TransitionNode::Reset()
	{
		m_currentTime = 0.f;
		isFinish = false;
	}

	void TransitionNode::Destroy(std::vector<AnimationNode*>& nodeToDestroy)
	{
		if (m_nextAnimationNode)
			m_nextAnimationNode->Destroy(nodeToDestroy);
	}

	Core::Maths::Mat4 AnimationNode::BlendAnimation(const std::shared_ptr<Bone>& bone, Core::Datastructure::Transform& out)
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

		float weight = static_cast<float>((m_currentTime - boneAnim->timeKey[curr]) / (boneAnim->timeKey[next] - boneAnim->timeKey[curr]));


		Core::Maths::Vec3 position{ (currLocal.localPosition) * (1 - weight) + (nextLocal.localPosition) * weight };

		Core::Maths::Quat rotation{ Nlerp(currLocal.localRotation,nextLocal.localRotation, weight) };

		out = Core::Datastructure::Transform{ position, rotation, currLocal.localScale };
		return out.GetLocalTrs();
	}


	unsigned int AnimationNode::FindBoneAnimationIndex(const std::shared_ptr<BoneAnimation>& boneAnim)
	{
		unsigned int index;
		for (index = 0; index < boneAnim->timeKey.size() - 1; index++)
		{
			if (m_currentTime < boneAnim->timeKey[index + 1])
				return index;
		}
		return index;
	}

	void AnimationNode::UpdateAnimationBone(const std::shared_ptr<Bone>& currentBone, const Core::Maths::Mat4& parent, std::vector<float>& finalTRSfloat)
	{
		Core::Maths::Mat4 nodeTransform{ currentBone->baseTransform.GetLocalTrs() };
		if (currentBone->activeBone)
		{
			if (!m_inTransition)
			{
				if (nodeAnimation->animationTree.count(currentBone->boneName) > 0)
				{
					Core::Datastructure::Transform TRS;
					nodeTransform = BlendAnimation(currentBone, TRS);
				}
			}
			else
				nodeTransform = transitionsAnimation[indexTransition]->UpdateBoneTransition(currentBone);
		}

		Core::Maths::Mat4 globalTransform = parent * nodeTransform;
		if (currentBone->activeBone)
		{
			Core::Maths::Mat4 finalTransform = (globalTransform * (currentBone->offsetBone));

			int currentPosInArrayFloat = currentBone->boneIndex * 16;
			for (int i = 0; i < 16; i++)
			{
				finalTRSfloat[currentPosInArrayFloat + i] = finalTransform.array[i];
			}
		}

		for (auto i{ 0 }; i < currentBone->child.size(); i++)
		{
			UpdateAnimationBone(currentBone->child[i], globalTransform, finalTRSfloat);
		}
	}

	void AnimationNode::UpdateAnimationNode(float deltaTime)
	{

		if (m_inTransition)
		{
			transitionsAnimation[indexTransition]->UpdateTimer(deltaTime);
			if (!transitionsAnimation[indexTransition]->m_nextAnimationNode)
			{
				transitionsAnimation[indexTransition]->Reset();
				m_inTransition = false;
			}
		}
		else
		{
			UpdateTimer(deltaTime);

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
		if (reset)
		{
			m_currentTime = 0.f;
			reset = false;
		}

		m_currentTime = m_currentTime + deltaTime * speed;

		if (m_currentTime > nodeAnimation->Time)
		{
			if (loop)
			{
				reset = true;
			}
			m_currentTime = nodeAnimation->Time;
		}
	}

	bool AnimationNode::DefaultConditionAnimationNode()
	{
		if (m_currentTime >= nodeAnimation->Time || reset)
			return true;
		else
			return false;
	}

	bool AnimationNode::IsTransitionFinish()
	{
		if (m_inTransition && transitionsAnimation.size() > indexTransition)
			return transitionsAnimation[indexTransition]->IsTransitionFinish();
		else
			return false;
	}

	bool AnimationNode::IsTimeIsTheSame()
	{
		return m_currentTime == m_lastTime;
	}

	bool AnimationNode::IsInTransition()
	{
		return m_inTransition;
	}

	void AnimationNode::SetNextAnimation(AnimationNode*& currentAnimation)
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

	void AnimationNode::Destroy(std::vector<AnimationNode*>& nodeToDestroy)
	{
		for (size_t i = 0; i < nodeToDestroy.size(); i++)
		{
			if (nodeToDestroy[i] == this)
				return;
		}
		nodeToDestroy.push_back(this);
		for (auto it = transitionsAnimation.begin(); it != transitionsAnimation.end();)
		{
			if ((*it))
			{
				(*it)->Destroy(nodeToDestroy);
				delete (*it);
				(*it) = nullptr;
				it = transitionsAnimation.erase(it);
			}
			else
				++it;
		}
	}

	AnimationHandler::AnimationHandler(AnimationNode* animationNode):
		m_currentAnimationNode{ animationNode }
	{}

	AnimationHandler::~AnimationHandler()
	{
		if (m_currentAnimationNode)
		{
			std::vector<AnimationNode*> toDestroy;
			m_currentAnimationNode->Destroy(toDestroy);

			for (size_t i{ 0 }; i < toDestroy.size(); i++)
			{
				if (toDestroy[i])
				{
					delete toDestroy[i];
					toDestroy[i] = nullptr;
				}
			}
			toDestroy.clear();
		}
	}

	AnimationHandler& AnimationHandler::operator=(const AnimationHandler& animation)
	{
		m_currentAnimationNode = animation.m_currentAnimationNode;
		animationFinish = animation.animationFinish;

		return *this;
	}

	void AnimationHandler::UpdateSkeletalMeshBones(const std::shared_ptr<Bone>& rootBone, std::vector<float>& finalTRSfloat, float deltaTime)
	{
		if (m_currentAnimationNode && m_currentAnimationNode->nodeAnimation)
		{
			if (!m_currentAnimationNode->IsTimeIsTheSame() || m_currentAnimationNode->IsInTransition())
			{
				Core::Maths::Mat4 identity{ identity.Identity() };
				for (auto i{ 0 }; i < rootBone->child.size(); i++)
				{
					m_currentAnimationNode->UpdateAnimationBone(rootBone->child[i], identity, finalTRSfloat);
				}
			}

			m_currentAnimationNode->UpdateAnimationNode(deltaTime);

			// check if the Transition of the currentAniation is finish
			if (m_currentAnimationNode->IsTransitionFinish())
			{
				m_currentAnimationNode->SetNextAnimation(m_currentAnimationNode);
			}
		}

		animationFinish = true;
	}

	void AnimationHandler::PlayAnimation(AnimationNode* animation)
	{
		m_currentAnimationNode = animation;
	}

}