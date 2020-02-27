#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

#include "OffsetMesh.h" 
#include "Vertex.h" 
#include "OpenGlLinkState.h" 
#include "Debug.h"

namespace Resources
{
	struct ModelData
	{
		GLuint VAOModel{0};
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<OffsetMesh> offsetsMesh;
		std::vector<std::shared_ptr<Material>> materialsModel;
		std::string materialModelName;
		EOpenGLLinkState stateVAO;

	};
}