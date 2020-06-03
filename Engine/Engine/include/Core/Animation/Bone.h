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

		public:
			bool									activeBone{ false };

			unsigned int							boneIndex{ 0 };
			std::string								boneName;

			Core::Maths::Mat4						offsetBone;
			Core::Datastructure::Transform			baseTransform;
			Core::Datastructure::Transform			localTRS;
			std::vector<std::shared_ptr<Bone>>		child;
		
		public:
			/**
			 * Init a bone with the node of assimp
			 * @param node: the current node
			 * @param bonesIndex: the map of the bone information
			 */
			void InitBone(const aiNode* node, const std::shared_ptr<Resources::unorderedmapBonesIndex>& bonesIndex);

		};

		struct BoneTree
		{
		public:
			std::shared_ptr<Bone> rootBone{};
			unsigned int numActiveBones;
		};
	}
}

