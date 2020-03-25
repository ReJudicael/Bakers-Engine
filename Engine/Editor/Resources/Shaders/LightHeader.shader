struct Material
{
	vec3	diffuseColor;
	vec3	ambientColor;
	vec3	specularColor;
	float	shininess;
	float	shininessStrength;
};

struct light
{
	int type;
	float angle;
	float range;
	float anglesmoothness;
    vec3 position;
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

vec3 getLightContribution(light light, Material mat, vec3 pos, vec3 normal)
{
	vec3 lightDir;
	float attenuation = 1;
	if (light.type == 0)
		lightDir = normalize(light.direction);
	else
	{
		vec3 t;
		t.x = 1;
		
		lightDir = (light.position - pos);
		float distance = length(lightDir);
		lightDir = normalize(lightDir);
		
		if (distance > light.range)
			attenuation = 0;
		else 
		{
		attenuation = 1 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
		if (light.type == 2)
		{
			vec3 spotDir = normalize(-light.direction);
			float angle = acos(dot(lightDir, spotDir));
			if (angle >= (light.angle * 0.5) - light.anglesmoothness)
			{
				if (light.anglesmoothness == 0 || angle >= (light.angle * 0.5) + light.anglesmoothness)
					attenuation = 0;
				else
				{
				float lerpValue = (angle - (light.angle * 0.5) + light.anglesmoothness);
				lerpValue /= (light.anglesmoothness * 2);
				//if (lerpValue < 1)
					//return t;
				attenuation = mix(attenuation, 0, lerpValue);
				}
			}
		}
		}
	}

	vec3 ambient = attenuation * light.ambient * mat.ambientColor;

	float diff = dot(normal, lightDir);
	if (diff < 0)
		diff = 0;
	vec3 diffuse = attenuation * diff * light.diffuse * mat.diffuseColor;

	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = dot(pos, reflectDir);
        if (spec > 0)
                spec = pow(spec, mat.shininess / 4.0);
        else
                spec = 0;
        vec3 specular = attenuation * spec * light.specular * mat.specularColor;
		specular = clamp(specular * mat.shininessStrength, 0.0, 1.0);

	return ambient + diffuse;// + specular;
}

