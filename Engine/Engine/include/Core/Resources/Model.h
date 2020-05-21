#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "OpenGlLinkState.h"
#include "Vertex.h"

namespace Resources
{
	struct Material;
	struct OffsetMesh;

	/*
	 * Contains the values with which we can draw the geometry of the mesh
	 */
	BAKERS_API_STRUCT Model
	{
		GLuint						VAOModel{ 0 };

		std::vector<OffsetMesh>		offsetsMesh;

		EOpenGLLinkState			stateVAO;

		Core::Maths::Vec3			min;
		Core::Maths::Vec3			max;
		std::vector<Vertex>			vertices;
		std::vector<GLuint>			indices;
		bool						haveBones;
	};
}