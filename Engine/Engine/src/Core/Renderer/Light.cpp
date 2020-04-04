#include <string>

#include "Light.h"
#include "Shader.h"
#include "Object.hpp"
#include "Quaternion.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
    using namespace Core::Renderer;
    registration::class_<Light>("Light")
        .constructor()
        .enumeration<Light::ELightType>("LightType")
        (
            value("Directional", Light::ELightType::DIRECTION),
            value("Point", Light::ELightType::POINT),
            value("Spot", Light::ELightType::SPOT)
        )
        .property("Active", &Light::m_isActive)
        .property("Type", &Light::m_type)
        .property("Range", &Light::m_range)
        .property("Angle", &Light::m_angle)
        .property("Angle smoothness", &Light::m_angleSmoothness)
        .property("Ambient", &Light::m_ambient)
        .property("Diffuse", &Light::m_diffuse)
        .property("Specular", &Light::m_specular)
        .property("Attenuation", &Light::m_attenuation);
}

namespace Core::Renderer
{
    void Light::OnCopy(IComponent* copyTo) const
    {
        ComponentBase::OnCopy(copyTo);
        Light* copy{ dynamic_cast<Light*>(copyTo) };

        copy->m_isActive = m_isActive;
        copy->m_type = m_type;
        copy->m_range = m_range;
        copy->m_angle = m_angle;
        copy->m_angleSmoothness = m_angleSmoothness;
        copy->m_ambient = m_ambient;
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

    void    Light::OnDestroy()
    {
        m_isActive = false;
        for (auto it{ Resources::Shader::lights.begin() }; it != Resources::Shader::lights.end(); ++it)
        {
            if (*it == this)
            {
                it = Resources::Shader::lights.erase(it);
                return;
            }
        }
    }
}