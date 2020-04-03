#pragma once

#include "ComponentBase.h"
#include "Color.hpp"

#include "CoreMinimal.h"

namespace NRenderer
{

	/**
	 * Light Component
	 */
	BAKERS_API_CLASS Light : public Core::Datastructure::ComponentBase
	{
	public :
		enum class ELightType
		{
			DIRECTION = 0,
			POINT,
			SPOT
		};

	private:
		bool				m_isActive = true;
		ELightType			m_type = ELightType::DIRECTION;
		float				m_range;
		float				m_angle;
		float				m_angleSmoothness;
		Core::Maths::Color	m_color;
		Core::Maths::Color	m_ambiant;
		Core::Maths::Color	m_diffuse;
		Core::Maths::Color	m_specular;
		Core::Maths::Color	m_attenuation;
	protected:
		virtual void	StartCopy(void*& copyTo) const override;
		virtual void	OnCopy(void* copyTo) const override;
		virtual void	OnStart() override;
		virtual void	OnDestroy() override;
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

		inline bool IsActive() const { return m_isActive; };
		inline ELightType GetLightType() const { return m_type; };
		inline float GetRange() const { return m_range; };
		inline float GetAngle() const { return m_angle; };
		inline float GetAngleSmoothness() const { return m_angleSmoothness; };
		inline Core::Maths::Color GetColor() const { return m_color; };
		inline Core::Maths::Color GetAmbiant() const { return m_ambiant; };
		inline Core::Maths::Color GetDiffuse() const { return m_diffuse; };
		inline Core::Maths::Color GetSpecular() const { return m_specular; };
		inline Core::Maths::Color GetAttenuation() const { return m_attenuation; };

		inline void SetLightType(ELightType value) { m_type = value; };
		inline void SetRange(const float value) { m_range = value; };
		inline void SetAngle(const float value) { m_angle = value; };
		inline void SetAngleSmoothness(const float value) { m_angleSmoothness = value; };
		inline void SetColor(const Core::Maths::Color& value) { m_color = value; };
		inline void SetAmbiant(const Core::Maths::Color& value) { m_ambiant = value; };
		inline void SetDiffuse(const Core::Maths::Color& value) { m_diffuse = value; };
		inline void SetSpecular(const Core::Maths::Color& value) { m_specular = value; };
		inline void SetAttenuation(const Core::Maths::Color& value) { m_attenuation = value; };

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

		REGISTER_CLASS(ComponentBase)
	};
};

