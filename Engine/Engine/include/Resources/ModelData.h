#pragma once
#include <GLFW/glfw3.h>
#include "OffsetMesh.h" 
#include <vector>

namespace Resources
{
	struct ModelData
	{
		GLuint VAOModel;
		std::vector<OffsetMesh> OffsetsMesh;
	};
}