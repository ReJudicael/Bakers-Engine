#include <string>

#include "Light.h"
#include "Shader.h"
#include "Object.hpp"
#include "Quaternion.hpp"
#include "Camera.h"

RTTR_PLUGIN_REGISTRATION
{
    ZoneScopedN("Registering RTTR")
        ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
    using namespace Core::Renderer;
    using namespace Core::Datastructure;

    RegisterDefaultClassConstructor<Light>("Light" , ComponentBase());

    registration::class_<Light>("Light")
        .enumeration<Light::ELightType>("LightType")
        (
            value("Directional", Light::ELightType::DIRECTION),
            value("Point", Light::ELightType::POINT),
            value("Spot", Light::ELightType::SPOT)
        );

    RegisterLuaEnumeration<Light>("Light", "LightType",
        "DIRECTION", Light::ELightType::DIRECTION, 
        "POINT", Light::ELightType::POINT,
        "SPOT", Light::ELightType::SPOT);

    RegisterClassProperty<Light>("Light", "Active", &Light::m_isActive);
    RegisterClassProperty<Light>("Light", "Type", &Light::m_type);
    RegisterClassProperty<Light>("Light", "Range", &Light::m_range);
    RegisterClassProperty<Light>("Light", "Angle", &Light::m_angle);
    RegisterClassProperty<Light>("Light", "AngleSmoothness", &Light::m_angleSmoothness);
    RegisterClassProperty<Light>("Light", "Ambient", &Light::m_ambient);
    RegisterClassProperty<Light>("Light", "Diffuse", &Light::m_diffuse);
    RegisterClassProperty<Light>("Light", "Specular", &Light::m_specular);
    RegisterClassProperty<Light>("Light", "Attenuation", &Light::m_attenuation);
    RegisterClassProperty<Light>("Light", "CastShadows", &Light::m_castShadow);
}

namespace Core::Renderer
{
    void Light::OnCopy(IComponent* copyTo) const
    {
        ZoneScoped
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

        copy->m_castShadow = m_castShadow;
        copy->m_depthBuffer = m_depthBuffer;
        copy->m_depthTexture = m_depthTexture;
        copy->m_depthWidth = m_depthWidth;
        copy->m_depthHeight = m_depthHeight;
    }

    void Light::StartCopy(IComponent*& copyTo) const
    {
        ZoneScoped
        copyTo = new Light();
        OnCopy(copyTo);
    }

    bool Light::OnStart()
    {
        ZoneScoped
        return ComponentBase::OnStart();
    }

    Light::Light() : ComponentBase()
    {
    }

    Light::~Light()
    {
        glDeleteTextures(1, &m_depthTexture);
        glDeleteFramebuffers(1, &m_depthBuffer);
    }

    void    Light::OnInit()
    {
        ZoneScoped
        Resources::Shader::lights.emplace_back(this);

        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            1000, 1000, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glGenFramebuffers(1, &m_depthBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_depthBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool Light::CanCastShadow()
    {
        return (m_type != ELightType::POINT && m_castShadow);
    }

    void Light::ResizeShadowTexture(unsigned width, unsigned height)
    {
        if (width == m_depthWidth && height == m_depthHeight)
            return;

        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        m_depthWidth = width;
        m_depthHeight = height;
    }

    Core::Maths::Vec3 Light::GetPosition()
    {
        return m_parent->GetGlobalPos();
    }

    Core::Maths::Vec3 Light::GetDirection()
    {
        return m_parent->Forward();
    }

    Core::Maths::Mat4 Light::GetViewFromLight()
    {
        if (m_type != ELightType::DIRECTION)
        {
            return m_parent->GetGlobalTRS().Inversed();
        }
        else
        {
            Datastructure::Transform view;
            view.SetLocalPos(GetDirection() * 15.f);
            view.SetLocalRot(m_parent->GetRot());
            return view.GetLocalTrs().Inversed();
        }
    }

    Core::Maths::Mat4 Light::GetProjFromLight()
    {
        switch (m_type)
        {
            case ELightType::DIRECTION:
                return Camera::CreateOrthographicMatrix(static_cast<float>(m_depthWidth), static_cast<float>(m_depthHeight), 0.1f, 100.f);
                break;
            case ELightType::POINT:
                return Core::Maths::Mat4::Identity();
                break;
            case ELightType::SPOT:
                return Camera::CreatePerspectiveMatrix(static_cast<float>(m_depthWidth), static_cast<float>(m_depthHeight), 60.f, 0.1f, 100.f);
                break;
            default:
                return Core::Maths::Mat4::Identity();
        }
    }

    void    Light::OnDestroy()
    {
        ComponentBase::OnDestroy();
        m_isActive = false; 
        for (auto it{ Resources::Shader::lights.begin() }; it != Resources::Shader::lights.end(); ++it)
        {
            if (*it == this)
            {
                it = Resources::Shader::lights.erase(it);
                break;
            }
        }
    }

    void    Light::OnReset()
    {
        ComponentBase::OnReset();
        
        for (auto it{ Resources::Shader::lights.begin() }; it != Resources::Shader::lights.end(); ++it)
        {
            if (*it == this)
            {
                it = Resources::Shader::lights.erase(it);
                break;
            }
        }

        Resources::Shader::lights.emplace_back(this);

        m_isActive = true;
        m_type = ELightType::DIRECTION;
        m_range = 100;
        m_angle = 0.785f;
        m_angleSmoothness = 0.001f;
        m_ambient = { 1.f, 1.f, 1.f };
        m_diffuse = { 1.f, 1.f, 1.f };
        m_specular = { 1.f, 1.f, 1.f };
        m_attenuation = { 1.f, 0.f, 0.f };

        m_castShadow = true;
    }

    bool    Light::IsActive() const
    {
        return m_isActive && m_parent->IsActive();
    }
}