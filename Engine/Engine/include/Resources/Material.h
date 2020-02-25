#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "Vec3.hpp"
#include "TextureData.h"


namespace Resources
{
	struct Material
	{
		Core::Maths::Vec3 diffuseColor;
		Core::Maths::Vec3 ambientColor;
		Core::Maths::Vec3 specularColor;

		Core::Maths::Vec3 materialColor;

		std::vector<std::shared_ptr<TextureData>> textures;
	};
}

