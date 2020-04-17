#pragma once

#include <array>

#include "DetourNavMesh.h"

namespace Core::Navigation
{
	struct NavPath
	{
		static constexpr int MAX_PATH_POLY = 64;
		std::array<dtPolyRef, MAX_PATH_POLY>				polyRefPath;
		int													numPolys;
		std::array<Core::Maths::Vec3, MAX_PATH_POLY * 3>	straightPath;
		std::array<dtPolyRef, MAX_PATH_POLY>				polyRefStraightPath;
		int													straightPathSize;
	};
}