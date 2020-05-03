#pragma once

#include "CoreMinimal.h"
#include "Transform.hpp"
#include <unordered_map>
#include <string>

struct aiAnimation;

namespace Core::Animation
{
	struct BoneLocalTransform
	{
		Core::Maths::Vec3 localPosition;
		Core::Maths::Quat localRotation;
		Core::Maths::Vec3 localScale;
	};

	struct BoneAnimation
	{
		std::vector<Core::Datastructure::Transform>		boneTransforms;
		std::vector<BoneLocalTransform>					boneLocalTransforms;
		std::vector<double>								timeKey;
	};

	struct Animation
	{
		float Time;
		float speed;
		unsigned int nbKeyFrame;
		std::string name;

		std::unordered_map<std::string, BoneAnimation> animationTree;

		void initAnimation(const aiAnimation* anim);
	};
}

