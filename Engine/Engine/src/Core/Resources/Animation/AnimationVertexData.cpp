#include "AnimationVertexData.h"

namespace Resources::Animation
{
	void AnimationVertexData::AddBoneData(const unsigned int& boneIndex, float weight)
	{
		for (unsigned int i{ 0 }; i < NB_BONES_PER_VERTEX; i++) 
		{
			if (weights[i] == 0.f)
			{
				boneIDs[i] = boneIndex;
				/*if (weight == 1.f && i != 0)
				{
					for (int j{ 0 }; j < i; j++)
					{
						weights[j] =  1.f / (i + 1);
					}
					weights[i] = 1.f / (i + 1);
				}
				else*/
				weights[i] = weight;
					//weights[i] = static_cast<int>(weight * 100) / 100.f;

				return;
			}
			else if (weights[i] == 1.f)
			{
				boneIDs[i] = boneIndex;
				weights[i] = weight;
				return;
			}
		}
	}
}