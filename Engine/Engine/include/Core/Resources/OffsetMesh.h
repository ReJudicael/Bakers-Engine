#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CoreMinimal.h"

namespace Resources
{

	/**
	 * Contains the values for draw the model with indices
	 */
	struct OffsetMesh
	{
		unsigned int materialIndices;
		unsigned int count;
		GLuint beginIndices;

	};
}