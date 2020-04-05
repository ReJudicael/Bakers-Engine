#pragma once

#include "CoreMinimal.h"

namespace Resources
{
	/**
	 * State of the class who need to be link with OpenGL
	 */
	enum class EOpenGLLinkState : int
	{
		CANTLINK = 0,
		LOADPROBLEM,
		CANLINK ,
		ISLINK,
	};
}