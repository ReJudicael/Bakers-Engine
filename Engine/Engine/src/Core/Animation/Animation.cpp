#include "Animation.h"
#include "Assimp/anim.h"

namespace Core::Animation
{

	void Animation::initAnimation(const aiAnimation* anim)
	{
		ZoneScoped
		name = anim->mName.data;

		Time = static_cast<float>(anim->mDuration);
		
		for (unsigned int i{ 0 }; i < anim->mNumChannels; i++)
		{
			aiNodeAnim* nodeAnim = anim->mChannels[i];

			std::shared_ptr<BoneAnimation> bone = std::make_shared<BoneAnimation>();
			//BoneAnimation bone;

			for (unsigned j{ 0 }; j < nodeAnim->mNumPositionKeys; j++)
			{
				BoneLocalTransform local;
				aiVectorKey vect = nodeAnim->mPositionKeys[j];
				Core::Maths::Vec3 pos{ vect.mValue.x, vect.mValue.y, vect.mValue.z };
				local.localPosition = { vect.mValue.x, vect.mValue.y, vect.mValue.z };
				
				bone->timeKey.push_back(vect.mTime);

				aiQuatKey quat = nodeAnim->mRotationKeys[j];
				Core::Maths::Quat rot{ quat.mValue.w, quat.mValue.x, quat.mValue.y, quat.mValue.z };
				local.localRotation = { quat.mValue.w, quat.mValue.x, quat.mValue.y, quat.mValue.z };

				vect = nodeAnim->mScalingKeys[j];
				Core::Maths::Vec3 scal{ vect.mValue.x, vect.mValue.y, vect.mValue.z };
				local.localScale = { vect.mValue.x, vect.mValue.y, vect.mValue.z };

				bone->boneTransforms.push_back(Core::Datastructure::Transform{ pos, rot, scal });
				bone->boneLocalTransforms.push_back(local);
			}

			animationTree.emplace(nodeAnim->mNodeName.data, bone);
		}
	}
}