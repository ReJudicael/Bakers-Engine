#pragma once

#include "Vec3.hpp" 
#include "Vec2.hpp" 
#include "CoreMinimal.h"

struct Vertex
{
	Core::Maths::Vec3 Position;
	Core::Maths::Vec2 UV;
	Core::Maths::Vec3 Normal;
};