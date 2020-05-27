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
}


namespace Core
{
	namespace Animation
	{

		struct Bone
		{

			Core::Datastructure::Transform			baseTransform;
			Core::Datastructure::Transform			LocalTRS;

			Core::Maths::Mat4						offsetBone;

			std::string								boneName;
			unsigned int							boneIndex;

			std::vector<std::shared_ptr<Bone>>		child;

			/**
			 * Init a bone with the node of assimp
			 * @param node: the current node
			 * @param bonesIndex: the map of the bone information
			 */
			void InitBone(const aiNode* node, const std::shared_ptr<Resources::unorderedmapBonesIndex>& bonesIndex);

		};

		struct BoneTree
		{
			std::shared_ptr<Bone> rootBone{};
			unsigned int numBone;
		};
	}
}

