#pragma once
#include "CoreMinimal.h"

#include "Bone.h"
#include "Animation.h"
#include <atomic>

namespace Core::Animation
{
	BAKERS_API_CLASS AnimationNode;

	BAKERS_API_CLASS TransitionNode
	{
	private:
		float m_currentTime;
		std::atomic<bool> isFinish{ false };

	public:
		std::shared_ptr<AnimationNode>	m_currentAnimationNode;
		std::function<bool()>			m_conditionTransition;
		std::shared_ptr<AnimationNode>	m_nextAnimationNode;
		float speed{ 20.f };

	public:
		/**
		 * The default constructor
		 */
		TransitionNode() = default;

		/**
		 * constructor for create a transition
		 */
		TransitionNode(std::shared_ptr<AnimationNode> currentAnimationNode,
			std::shared_ptr<AnimationNode> nextAnimationNode, float speedTrans,
			std::function<bool()> conditionTransition = nullptr);
		/**
		 * init the transition
		 * @param currentAnimationNode: the AnimationNode who have the transition
		 * @param nextAnimationNode: the AnimationNode with what the current transition with
		 * and who replace the current
		 * @param conditionTransition: the condition for the transition
		 */
		void InitTransition(std::shared_ptr<AnimationNode> currentAnimationNode, 
							std::shared_ptr<AnimationNode> nextAnimationNode, 
							std::function<bool()> conditionTransition = nullptr);

		/**
		 * Blend the 2 animation of the transition
		 * @param bone: the current bone
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 TransitionAnimation(std::shared_ptr<Bone> bone);
		
		/**
		 * Check if the bone can be blend
		 * @param bone: the current bone
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 UpdateBoneTransition(std::shared_ptr<Bone> bone);

		/**
		 * update the transition time
		 * @param deltaTime: the current deltaTime
		 */
		void UpdateTimer(float deltaTime);

		/**
		 * @return true if the transition is finish
		 */
		inline bool IsTransitionFinish()
		{
			return isFinish;
		}

		/**
		 * reset the transition
		 */
		void Reset();
	};



	BAKERS_API_CLASS AnimationNode
	{

	private:
		float m_currentTime;
		std::atomic<bool> m_inTransition{ false };
		unsigned int indexTransition;

	public:
		bool Loop{ true };
		std::atomic<bool> reset{ false };
		float speed{ 20.f };
		std::shared_ptr<Animation>						nodeAnimation;
		std::vector<std::shared_ptr<TransitionNode>>	transitionsAnimation;

	public:

		/**
		 * The default constructor
		 */
		AnimationNode() = default;

		/*
		 * Update the TRS matrix of the bone with the animation
		 * @param currentBone: the current bone
		 * @param parent: the parent TRS matrix
		 * @param finalTransform: the matrix vector wich is send to the shader
		 */
		void UpdateAnimationBone(	std::shared_ptr<Bone> currentBone, Core::Maths::Mat4 parent, 
									std::vector<Core::Maths::Mat4>& finalTransform);
		
		/*
		 * Blend the rotation and the position of the bone
		 * @param boneAnim: the current bone
		 * @param out: the Transform of the blend
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 BlendAnimation(std::shared_ptr<Bone> boneAnim, Core::Datastructure::Transform& out);
		
		/*
		 * Find the current keyBone of the bone in the animation
		 * @param boneAnim: the current bone
		 * @return the keyBone in the animation
		 */
		unsigned int FindBoneAnimationIndex(std::shared_ptr<BoneAnimation> boneAnim);

		/*
		 * Update the AnimationBone
		 * @param deltaTime: the current deltaTime
		 */
		void UpdateAnimationNode(float deltaTime);

		/*
		 * Update the timer of the AnimationBone
		 * @param deltaTime: the current deltaTime
		 */
		void UpdateTimer(float deltaTime);

		/*
		 * The default function use for the conditonFunction in the TransitionNode
		 * @return true if the Animation is finished
		 */
		bool DefaultConditionAnimationNode();

		/*
		 * Check if the transition is finished
		 * @return true if the transition is finished
		 */
		bool IsTransitionFinish()
		{
			if (m_inTransition && transitionsAnimation.size() > indexTransition)
				return transitionsAnimation[indexTransition]->IsTransitionFinish();
			else
				return false;
		}

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param currentAnimation: the currentAnimationNode in the AnimationHandler
		 */
		void SetNextAnimation(std::shared_ptr<AnimationNode>& currentAnimation);
	};



	BAKERS_API_CLASS AnimationHandler
	{
	private:
		std::shared_ptr<AnimationNode> m_currentAnimationNode;

	public:
		bool animationFinish{ true };

	public:
		/*
		 * The default construtor
		 */
		AnimationHandler() = default;

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param animationNode: the animationNode who is going
		 * the currentAnimationNode in the AnimationHandler
		 */
		AnimationHandler(std::shared_ptr<AnimationNode> animationNode);

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param currentAnimation: the currentAnimationNode in the AnimationHandler
		 */
		void UpdateSkeletalMeshBones(	std::shared_ptr<Bone> rootBone, 
										std::vector<Core::Maths::Mat4>& finalTransform, float deltaTime);

		void PlayAnimation(std::shared_ptr<AnimationNode> animation);
	};
}

