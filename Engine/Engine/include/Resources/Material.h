#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include "Vec3.hpp"


namespace Resources
{
	struct Material
	{
		Core::Maths::Vec3 diffuseColor;
		Core::Maths::Vec3 ambientColor;
		Core::Maths::Vec3 specularColor;

		Core::Maths::Vec3 materialColor;

		std::vector<GLuint> textures;

	};
}

