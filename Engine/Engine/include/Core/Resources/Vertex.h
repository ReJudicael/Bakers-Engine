#pragma once

#include "Vec3.hpp" 
#include "Vec2.hpp" 
#include "CoreMinimal.h"

/*
 * Contain the values for create the shape and apply light of a Model
 */
struct Vertex
{
	Core::Maths::Vec3 Position;
	Core::Maths::Vec2 UV;
	Core::Maths::Vec3 Normal;
	Core::Maths::Vec3 Tangent;
};