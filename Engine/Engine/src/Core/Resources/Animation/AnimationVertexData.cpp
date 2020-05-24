#include "AnimationVertexData.h"

namespace Resources::Animation
{
	void AnimationVertexData::AddBoneData(const unsigned int& boneIndex, float weight)
	{
		for (unsigned int i{ 0 }; i < NB_BONES_PER_VERTEX; i++) 
		{
			if (weights[i] == 0.f)
			{
				boneIDs[i] = static_cast<float>(boneIndex);
				weights[i] = weight;
				return;
			}
			else if (weights[i] == 1.f)
			{
				boneIDs[i] = static_cast<float>(boneIndex);
				weights[i] = weight;
				return;
			}
		}
	}
}