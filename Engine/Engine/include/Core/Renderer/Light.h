#pragma once

#include "ComponentBase.h"
#include "Color.hpp"

#include "CoreMinimal.h"

namespace Core::Renderer
{
	/**
	 * Light Component
	 */
	BAKERS_API_CLASS Light : public Datastructure::ComponentBase
	{
	public :
		/**
		 * Type of light for light computing
		 */
		enum class ELightType
		{
			DIRECTION = 0,
			POINT,
			SPOT
		};

	private:
		bool				m_isActive = true;
		ELightType			m_type = ELightType::DIRECTION;
		float				m_range = 100;
		float				m_angle = 0.785f;
		float				m_angleSmoothness = 0.001f;
		Core::Maths::Color	m_ambient;
		Core::Maths::Color	m_diffuse;
		Core::Maths::Color	m_specular;
		Core::Maths::Vec3	m_attenuation = { 1.f, 0.f, 0.f };
	protected:
		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;
		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * First frame upon creation event
		 */
		virtual void	OnStart() override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;
		virtual void	OnReset() override;
	public:
		/**
		 * Default Constructor
		 */
		Light();

		/**
		 * Activation getter
		 * @return True is the light is active, false otherwise
		 */
		inline bool IsActive() const { return m_isActive; };
		/**
		 * Type getter
		 * @return Current light type
		 */
		inline ELightType GetLightType() const { return m_type; };
		/**
		 * Range getter
		 * @return Current light range
		 */
		inline float GetRange() const { return m_range; };
		/**
		 * Angle getter
		 * @return Current light angle
		 */
		inline float GetAngle() const { return m_angle; };
		/**
		 * Angle smoothness (fade around edges of light range) getter
		 * @return Current light angle smoothness
		 */
		inline float GetAngleSmoothness() const { return m_angleSmoothness; };
		/**
		 * Color getter
		 * @return Current light color
		 */
		//inline Core::Maths::Color GetColor() const { return m_color; };
		/**
		 * Ambiant getter
		 * @return Current light ambiant component
		 */
		inline Core::Maths::Color GetAmbient() const { return m_ambient; };
		/**
		 * Diffuse getter
		 * @return Current light diffuse component
		 */
		inline Core::Maths::Color GetDiffuse() const { return m_diffuse; };
		/**
		 * Specular getter
		 * @return Current light specular component
		 */
		inline Core::Maths::Color GetSpecular() const { return m_specular; };
		/**
		 * Attenuation getter
		 * @return Current light attenuation vector
		 */
		inline Core::Maths::Vec3 GetAttenuation() const { return m_attenuation; };

		/**
		 * Light type setter
		 * @param value: New type for the light
		 */
		inline void SetLightType(ELightType value) { m_type = value; };
		/**
		 * Light range setter
		 * @param value: New range for the light
		 */
		inline void SetRange(const float value) { m_range = value; };
		/**
		 * Light angle setter
		 * @param value: New angle for the light
		 */
		inline void SetAngle(const float value) { m_angle = value; };
		/**
		 * Light angle smoothness setter
		 * @param value: New angle smoothness for the light
		 */
		inline void SetAngleSmoothness(const float value) { m_angleSmoothness = value; };
		/**
		 * Light color setter
		 * @param value: New color for the light
		 */
		//inline void SetColor(const Core::Maths::Color& value) { m_color = value; };
		/**
		 * Light ambiant setter
		 * @param value: New ambiant component for the light
		 */
		inline void SetAmbient(const Core::Maths::Color& value) { m_ambient = value; };
		/**
		 * Light diffuse setter
		 * @param value: New diffuse component for the light
		 */
		inline void SetDiffuse(const Core::Maths::Color& value) { m_diffuse = value; };
		/**
		 * Light specular setter
		 * @param value: New specular component for the light
		 */
		inline void SetSpecular(const Core::Maths::Color& value) { m_specular = value; };
		/**
		 * Light attenuation setter
		 * @param value: New attenuation vector for the light
		 */
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

		REGISTER_CLASS(ComponentBase)
	};
};

