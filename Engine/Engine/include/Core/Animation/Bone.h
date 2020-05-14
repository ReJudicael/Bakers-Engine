#pragma once

#include "CoreMinimal.h"
#include "Vec3.hpp"
#include "Mat4.hpp"
#include "Quaternion.hpp"
#include "Transform.hpp"
#include "BoneData.h"

struct aiNode;

namespace Resources
{
	using unorderedmapBonesIndex = std::unordered_map<std::string, Animation::BoneData>;
	//using unorderedmapSkeletalIndex = std::unordered_map<std::string, std::shared_ptr<unorderedmapBonesIndex>>;
}


namespace Core
{
	namespace Animation
	{

		struct Bone
		{
			Core::Maths::Vec3				relativePosition;
			Core::Maths::Quat				relativeRotation;

			Core::Datastructure::Transform	baseTransform;
			Core::Datastructure::Transform	LocalTRS;

			Core::Maths::Mat4				offsetBone;

			std::string						boneName;
			unsigned int					boneIndex;

			//Core::Animation::Bone			parent;
			std::vector<Bone>				child;

			void InitBone(const aiNode* node, const std::shared_ptr<Resources::unorderedmapBonesIndex>& bonesIndex, Core::Datastructure::Transform offsetP);

		};

		struct BoneTree
		{
			Bone rootBone{};
			unsigned int numBone;
			Core::Maths::Mat4 inverseGlobal;
		};
	}
}

