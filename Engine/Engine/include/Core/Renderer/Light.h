#pragma once

#include "ComponentBase.h"
#include "Vec3.hpp"


namespace NRenderer
{

	/**
	 * Light Component
	 */
	class Light : public Core::Datastructure::ComponentBase
	{
	public :
		enum class ELightType
		{
			DIRECTION = 0,
			POINT,
			SPOT
		};

	private:

		ELightType			m_type = ELightType::DIRECTION;
		float				m_range;
		float				m_angle;
		float				m_angleSmoothness;
		Core::Maths::Vec3	m_color;
		Core::Maths::Vec3	m_ambiant;
		Core::Maths::Vec3	m_diffuse;
		Core::Maths::Vec3	m_specular;
		Core::Maths::Vec3	m_attenuation;

	public:
		/**
		 * Default Constructor
		 */
		Light();

		/**
		 * Copy Constructor
		 * @param other: Light to copy
		 */
		Light(const Light& other);

		/**
		 * Rvalue Constructor
		 * @param other: Light to move
		 */
		Light(Light&& other) noexcept;

		/**
		 * Assignment operator
		 * @param other: Light to copy
		 */
		Light& operator=(const Light& other);

		/**
		 * Assignment operator
		 * @param other: Light to move
		 */
		Light& operator=(Light&& other) noexcept;

		inline ELightType GetLightType() const { return m_type; };
		inline float GetRange() const { return m_range; };
		inline float GetAngle() const { return m_angle; };
		inline float GetAngleSmoothness() const { return m_angleSmoothness; };
		inline Core::Maths::Vec3 GetColor() const { return m_color; };
		inline Core::Maths::Vec3 GetAmbiant() const { return m_ambiant; };
		inline Core::Maths::Vec3 GetDiffuse() const { return m_diffuse; };
		inline Core::Maths::Vec3 GetSpecular() const { return m_specular; };
		inline Core::Maths::Vec3 GetAttenuation() const { return m_attenuation; };

		inline void SetLightType(ELightType value) { m_type = value; };
		inline void SetRange(const float value) { m_range = value; };
		inline void SetAngle(const float value) { m_angle = value; };
		inline void SetAngleSmoothness(const float value) { m_angleSmoothness = value; };
		inline void SetColor(const Core::Maths::Vec3& value) { m_color = value; };
		inline void SetAmbiant(const Core::Maths::Vec3& value) { m_ambiant = value; };
		inline void SetDiffuse(const Core::Maths::Vec3& value) { m_diffuse = value; };
		inline void SetSpecular(const Core::Maths::Vec3& value) { m_specular = value; };
		inline void SetAttenuation(const Core::Maths::Vec3& value) { m_attenuation = value; };

		/**
		 * Get Light position in World Space
		 * @return Position of owner in World Space
		 */
		Core::Maths::Vec3 GetPosition();

		/**
		 * Get Light direction in World Space
		 * @return Direction of owner's forward vector in World Space
		 */
		Core::Maths::Vec3 GetDirection();
	};

};

