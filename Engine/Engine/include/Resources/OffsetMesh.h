#pragma once
#include <GLFW/glfw3.h>

namespace Resources
{
	struct OffsetMesh
	{
		unsigned int materialIndices;
		unsigned int count;
		GLuint beginIndices;
	};
}