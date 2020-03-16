#pragma once

#include "CoreMinimal.h"

namespace Resources
{
	enum class EOpenGLLinkState : int
	{
		CANTLINK = 0,
		LOADPROBLEM,
		CANLINK ,
		ISLINK,
	};
}