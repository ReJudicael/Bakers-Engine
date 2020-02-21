#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "IComponent.hpp"

namespace Core::Datastructure
{
	class ICamera : public virtual IComponent
	{
	protected:
		//float m_theta = 0;
		//float m_phi = 0;

		Core::Maths::Vec3 m_front;
		Core::Maths::Vec3 m_left;

		Core::Maths::Mat4 m_perspective;
		Core::Maths::Mat4 m_cameraMatrix;
	};
}

