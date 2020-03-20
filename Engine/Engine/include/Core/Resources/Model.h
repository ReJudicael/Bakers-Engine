#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "OpenGlLinkState.h"

namespace Resources
{
	struct Material;
	struct OffsetMesh;

	/*
	 * Contains the values with which we can draw the geometry of the mesh
	 */
	struct Model
	{
		GLuint VAOModel{ 0 };

		std::vector<OffsetMesh> offsetsMesh;

		EOpenGLLinkState stateVAO;
	};
}