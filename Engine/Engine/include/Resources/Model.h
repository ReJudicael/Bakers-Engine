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

	struct Model
	{
		GLuint VAOModel{ 0 };

		std::vector<OffsetMesh> offsetsMesh;
		std::vector<std::shared_ptr<Material>> materialsModel;

		EOpenGLLinkState stateVAO;
	};
}