#include <string>

#include "Light.h"
#include "Shader.h"
#include "Object.hpp"
#include "Quaternion.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
    using namespace NRenderer;
    registration::class_<Light>("Light")
        .constructor()
        .enumeration<Light::ELightType>("LightType")
        (
            value("Directional", Light::ELightType::DIRECTION),
            value("Point", Light::ELightType::POINT),
            value("Spot", Light::ELightType::SPOT)
        )
        .property("Type", &Light::m_type)
        .property("Range", &Light::m_range)
        .property("Angle", &Light::m_angle)
        .property("Angle smoothness", &Light::m_angleSmoothness)
        .property("Color", &Light::m_color)
        .property("Ambiant", &Light::m_ambiant)
        .property("Diffuse", &Light::m_diffuse)
        .property("Attenuation", &Light::m_attenuation);
}

namespace NRenderer
{
    void Light::OnCopy(IComponent* copyTo) const
    {
        ComponentBase::OnCopy(copyTo);
        Light* copy{ dynamic_cast<Light*>(copyTo) };

        copy->m_type = m_type;
        copy->m_range = m_range;
        copy->m_angle = m_angle;
        copy->m_angleSmoothness = m_angleSmoothness;
        copy->m_color = m_color;
        copy->m_ambiant = m_ambiant;
        copy->m_diffuse = m_diffuse;
        copy->m_specular = m_specular;
        copy->m_attenuation = m_attenuation;
    }

    void Light::StartCopy(IComponent*& copyTo) const
    {
        copyTo = new Light();
        OnCopy(copyTo);
    }

    void Light::OnStart()
    {
        ComponentBase::OnStart();
        Resources::Shader::lights.emplace_back(this);
    }

    Light::Light() : ComponentBase()
    {
    }

    Light::Light(const Light& other) :
        ComponentBase(other),
        m_type{ other.m_type },
        m_color{other.m_color},
        m_ambiant{other.m_ambiant},
        m_diffuse{other.m_diffuse},
        m_specular{other.m_specular},
        m_attenuation{other.m_attenuation}
    {}

    Light::Light(Light&& other) noexcept :
        ComponentBase(std::move(other)),
        m_type{ std::move(other.m_type) },
        m_color{ std::move(other.m_color) },
        m_ambiant{ std::move(other.m_ambiant) },
        m_diffuse{ std::move(other.m_diffuse) },
        m_specular{ std::move(other.m_specular) },
        m_attenuation{ std::move(other.m_attenuation) }
    {}

    Light& Light::operator=(const Light& other)
    {
        ComponentBase::operator=(other);
        m_type = other.m_type;
        m_color = other.m_color;
        m_ambiant = other.m_ambiant;
        m_diffuse = other.m_diffuse;
        m_specular = other.m_specular;
        m_attenuation = other.m_attenuation;

        return *this;
    }

    Light& Light::operator=(Light&& other) noexcept
    {
        ComponentBase::operator=(std::move(other));
        m_type = std::move(other.m_type);
        m_color = std::move(other.m_color);
        m_ambiant = std::move(other.m_ambiant);
        m_diffuse = std::move(other.m_diffuse);
        m_specular = std::move(other.m_specular);
        m_attenuation = std::move(other.m_attenuation);

        return *this;
    }

    Core::Maths::Vec3 Light::GetPosition()
    {
        return m_parent->GetGlobalPos();
    }

    Core::Maths::Vec3 Light::GetDirection()
    {
        Core::Maths::Vec4 forward(0, 0, 1, 0);
        Core::Maths::Quat q = m_parent->GetGlobalRot();
        return (q * forward * q.Inversed()).xyz;
    }
}