#include "Bone.h"
#include "Assimp/scene.h"
#include "ResourcesManager.h"


namespace Core::Animation
{
	void Bone::InitBone(const aiNode* node, const std::shared_ptr<Resources::unorderedmapBonesIndex>& bonesIndex)
	{

		activeBone = { bonesIndex->count(node->mName.data) > 0 };
		if (activeBone)
		{
			Resources::Animation::BoneData currBoneData = bonesIndex->operator[](node->mName.data);

			offsetBone = currBoneData.offsetMesh;
			boneIndex = currBoneData.boneIndex;
		}
		boneName = node->mName.data;

		aiVector3D pos;
		aiQuaternion rot;
		aiVector3D sca;
		node->mTransformation.Decompose(sca, rot, pos);
		baseTransform = Core::Datastructure::Transform{ { pos.x, pos.y, pos.z }, 
														{ rot.w, rot.x, rot.y, rot.z },
														{ sca.x, sca.y, sca.z } };

		for (unsigned int i{ 0 }; i < node->mNumChildren; i++)
		{
			std::shared_ptr<Bone> childBone = std::make_shared<Bone>();
			const aiNode* childNode{ node->mChildren[i] };

			childBone->InitBone(childNode, bonesIndex);
			child.push_back(childBone);
		}
	}
}