#pragma once
#include "CoreMinimal.h"

#define NB_BONES_PER_VERTEX 4

#include "Vec3.hpp" 
#include "Vec2.hpp" 

namespace Resources::Animation
{
	struct AnimationVertexData
	{
		float boneIDs[NB_BONES_PER_VERTEX];
		float weights[NB_BONES_PER_VERTEX];

		void AddBoneData(const unsigned int& boneIndex, float weight);
	};
}

