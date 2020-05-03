#include "Bone.h"
#include "Assimp/scene.h"
#include "LoadResources.h"


namespace Core::Animation
{
	void Bone::InitBone(const aiNode* node, const std::shared_ptr<Resources::unorderedmapBonesIndex>& bonesIndex, Core::Datastructure::Transform offsetP)
	{
		Resources::Animation::BoneData currBoneData = bonesIndex->operator[](node->mName.data);

		offsetBone = /*offsetP */ currBoneData.offsetMesh;
		boneIndex = currBoneData.boneIndex;
		boneName = node->mName.data;

		aiVector3D pos;
		aiQuaternion rot;
		aiVector3D sca;
		node->mTransformation.Decompose(sca, rot, pos);
		baseTransform = Core::Datastructure::Transform{ offsetP,
														{ pos.x, pos.y, pos.z }, 
														{ rot.w, rot.x, rot.y, rot.z },
														{ sca.x, sca.y, sca.z } };
		//offsetBone = (offsetP * baseTransform.GetLocalTrs()).Inversed();


		for (unsigned int i{ 0 }; i < node->mNumChildren; i++)
		{
			Bone childBone;
			const aiNode* childNode{ node->mChildren[i] };
			if (bonesIndex->count(childNode->mName.data) > 0)
			{
				childBone.InitBone(childNode, bonesIndex, baseTransform);
				child.push_back(childBone);
			}
		}
	}
}