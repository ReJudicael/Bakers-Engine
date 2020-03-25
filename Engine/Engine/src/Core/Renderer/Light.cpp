#include <string>

#include "Light.h"
#include "Shader.h"
#include "Object.hpp"
#include "Quaternion.hpp"

std::string LightShaderContent = R"GLSL(

struct light
{
    vec3 viewPosition;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

vec3 getLightContribution(light light, vec3 pos, vec3 normal)
{
    vec3 lightDir = normalize(light.position);

	vec3 ambient = light.ambient * materialAmbient;

	float diff = dot(norm, lightDir);
	if (diff < 0)
		diff = 0;
	vec3 diffuse = diff * (light.diffuse * materialDiffuse);

	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = dot(view, reflectDir);
        if (spec > 0)
                spec = pow(spec, shininess);
        else
                spec = 0;
        vec3 specular = spec * (light.specular * materialSpecular);

	return ambient + diffuse + specular;
}

)GLSL";

namespace NRenderer
{
    Light::Light() : ComponentBase()
    {
        Resources::Shader::lights.emplace_back(this);
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