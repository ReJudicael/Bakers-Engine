#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "IComponent.hpp"

namespace Core::Datastructure
{
	class ICamera : public virtual IComponent
	{
	private:
		Core::Maths::Mat4	m_perspective;
		Core::Maths::Mat4	m_cameraMatrix;
	protected:
		//float m_theta = 0;
		//float m_phi = 0;

		Core::Maths::Vec3	m_front;
		Core::Maths::Vec3	m_left;

		bool				m_isPerspectiveUpdated{ false };
		bool				m_isCamUpdated{ false };

		virtual Core::Maths::Mat4	OnGeneratePerspective() = 0;
		virtual Core::Maths::Mat4	OnGenerateCamera() = 0;
	public:
		/**
		 * Get perspective matrix of the camera
		 * @return Stored perspective matrix
		 */
		const Core::Maths::Mat4&	GetPerspectiveMatrix();
		/**
		 * Get reverse TRS matrix of the camera
		 * @return Reverse TRS camera matrix
		 */
		const Core::Maths::Mat4&	GetCameraMatrix();

		virtual void				OnStart() override;

		virtual void				SetRatio(float ratio) = 0;
	};
}

