#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>

#include "Vec3.hpp"
#include "Texture.h"
#include "CoreMinimal.h"

namespace Resources
{
	struct Texture;
	struct Material
	{
		Core::Maths::Vec3 diffuseColor;
		Core::Maths::Vec3 ambientColor;
		Core::Maths::Vec3 specularColor;

		Core::Maths::Vec3 materialColor;

		std::shared_ptr<GLuint> program;

		std::vector<std::shared_ptr<Texture>> textures;
	};
}

