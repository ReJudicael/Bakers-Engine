#pragma once
#include "CoreMinimal.h"
#include "Mat4.hpp"

namespace Resources::Animation
{
	struct BoneData
	{
		unsigned int		boneIndex;
		Core::Maths::Mat4	offsetMesh;
	};
}

