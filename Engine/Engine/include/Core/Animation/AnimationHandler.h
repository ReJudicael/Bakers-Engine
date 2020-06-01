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
		AnimationNode*					m_currentAnimationNode{ nullptr };
		std::function<bool()>			m_conditionTransition;
		AnimationNode*					m_nextAnimationNode;
		float speed{ 10.f };

	public:
		/**
		 * The default constructor
		 */
		TransitionNode() = default;

		/**
		 * constructor for create a transition
		 */
		TransitionNode(AnimationNode* currentAnimationNode,
			AnimationNode* nextAnimationNode, float speedTrans,
			std::function<bool()> conditionTransition = nullptr);
		/**
		 * init the transition
		 * @param currentAnimationNode: the AnimationNode who have the transition
		 * @param nextAnimationNode: the AnimationNode with what the current transition with
		 * and who replace the current
		 * @param conditionTransition: the condition for the transition
		 */
		void InitTransition(AnimationNode* currentAnimationNode, 
							AnimationNode* nextAnimationNode, 
							std::function<bool()> conditionTransition = nullptr);

		/**
		 * Blend the 2 animation of the transition
		 * @param bone: the current bone
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 TransitionAnimation(const std::shared_ptr<Bone>& bone);
		
		/**
		 * Check if the bone can be blend
		 * @param bone: the current bone
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 UpdateBoneTransition(const std::shared_ptr<Bone>& bone);

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

		void Destroy(std::vector<AnimationNode*>& nodeToDestroy);
	};



	BAKERS_API_CLASS AnimationNode
	{

	private:
		std::atomic<float> m_currentTime;
		std::atomic<float> m_lastTime;
		std::atomic<bool> m_inTransition{ false };
		unsigned int indexTransition;

	public:
		bool Loop{ true };
		std::atomic<bool>				reset{ false };
		float							speed{ 20.f };
		std::shared_ptr<Animation>	nodeAnimation{ nullptr };
		std::vector<TransitionNode*>	transitionsAnimation;

	public:

		/**
		 * The default constructor
		 */
		AnimationNode() = default;

		/*
		 * Blend the rotation and the position of the bone
		 * @param boneAnim: the current bone
		 * @param out: the Transform of the blend
		 * @return the blended matrix
		 */
		Core::Maths::Mat4 BlendAnimation(const std::shared_ptr<Bone>& boneAnim, Core::Datastructure::Transform& out);

		/*
		 * Find the current keyBone of the bone in the animation
		 * @param boneAnim: the current bone
		 * @return the keyBone in the animation
		 */
		unsigned int FindBoneAnimationIndex(const std::shared_ptr<BoneAnimation>& boneAnim);

		/*
		 * Update the TRS matrix of the bone with the animation
		 * @param currentBone: the current bone
		 * @param parent: the parent TRS matrix
		 * @param finalTransform: the float vector wich is send to the shader
		 */
		void UpdateAnimationBone(const std::shared_ptr<Bone>& currentBone, const Core::Maths::Mat4& parent, std::vector<float> & finalTRSfloat);
		

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
		/**
		 * check if the current time of the AnimationNode is the max
		 * of the Animation time
		 * @return true if the current Time is the same as the last update
		 */
		inline bool IsTimeIsTheSame()
		{
			return { m_currentTime == m_lastTime };
		}

		/**
		 * Check if the AnimationNode is playing a transition
		 * @return true if the AnimationNode is playing a transition
		 */
		inline bool IsInTransition()
		{
			return m_inTransition;
		}

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param currentAnimation: the currentAnimationNode in the AnimationHandler
		 */
		void SetNextAnimation(AnimationNode*& currentAnimation);

		void Destroy(std::vector<AnimationNode*>& nodeToDestroy);
	};



	BAKERS_API_CLASS AnimationHandler
	{
	private:
		AnimationNode* m_currentAnimationNode{nullptr};

	public:
		bool animationFinish{ true };

	public:
		/*
		 * The default construtor
		 */
		AnimationHandler() = default;

		/*
		 * The default construtor
		 */
		~AnimationHandler();

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param animationNode: the animationNode who is going
		 * the currentAnimationNode in the AnimationHandler
		 */
		AnimationHandler(AnimationNode* animationNode);

		AnimationHandler& operator=(const AnimationHandler& animation);

		/*
		 * Set the new currentAnimationNode in the AnimationHandler
		 * @param currentAnimation: the currentAnimationNode in the AnimationHandler
		 */
		void UpdateSkeletalMeshBones(const std::shared_ptr<Bone>& rootBone, std::vector<float>& finalTRSfloat, float deltaTime);

		void PlayAnimation(AnimationNode* animation);

		void DestroyHandler();
	};
}

