#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CoreMinimal.h"

namespace Resources
{
	struct OffsetMesh
	{
		unsigned int materialIndices;
		unsigned int count;
		GLuint beginIndices;
	};
}