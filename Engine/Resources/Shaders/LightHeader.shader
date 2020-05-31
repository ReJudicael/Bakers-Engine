struct Material
{
	vec3	diffuseColor;
	vec3	ambientColor;
	vec3	specularColor;
	float	shininess;
	float	shininessStrength;
};

struct lightPOV
{
	mat4 view;
	mat4 proj;
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
	

float shadowComputing(int shadowCount, vec4[10] positions, sampler2D[10] shadowMaps)
{
	if (shadowCount == 0 || shadowCount > 10)
		return 1.0;

	float shadow = 0;

	for (int i = 0; i < shadowCount; ++i)
	{
		vec3 proj = positions[i].xyz / positions[i].w;
		proj = proj * 0.5 + 0.5;
	
		float currentDepth = proj.z;
		if (currentDepth > 1.0)
			return 0.0;
	
		// Shadow bias to remove shadow acne
		float bias = 0.0005;
	
		// Compute smooth shadow with PCF
		float currentShadow = 0;
		vec2 size = 1.0 / textureSize(shadowMaps[i], 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float depth = texture(shadowMaps[i], proj.xy + vec2(x, y) * size).r; 
				currentShadow += currentDepth - bias < depth ? 1.0 : 0.0;        
			}    
		}
		currentShadow /= 9.0;
	
		shadow += currentShadow;
	}
	
	return shadow / float(shadowCount);
}	
	
	
vec3 getLightContribution(light light, Material mat, vec3 pos, vec3 normal, vec3 view, float shadow)
{
	vec3 lightDir;
	float attenuation = 1;
	if (light.type == 0)
		lightDir = normalize(-light.direction);
	else
	{
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
			vec3 spotDir = normalize(light.direction);
			float angle = acos(dot(lightDir, spotDir));
			if (angle >= (light.angle * 0.5) - light.anglesmoothness)
			{
				if (light.anglesmoothness == 0 || angle >= (light.angle * 0.5) + light.anglesmoothness)
					attenuation = 0;
				else
				{
				float lerpValue = (angle - (light.angle * 0.5) + light.anglesmoothness);
				lerpValue /= (light.anglesmoothness * 2);
				attenuation = mix(attenuation, 0, lerpValue);
				}
			}
		}
		}
	}

	vec3 ambient = attenuation * light.ambient * mat.ambientColor * max(0.5, shadow);

	float diff = dot(normal, lightDir);
	if (diff < 0)
		diff = 0;
	vec3 diffuse = attenuation * diff * light.diffuse * mat.diffuseColor * shadow;

	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = dot(view, reflectDir);
        if (spec > 0)
                spec = pow(spec, mat.shininess / 4.0);
        else
                spec = 0;
        vec3 specular = attenuation * spec * light.specular * mat.specularColor;
		specular = clamp(specular * mat.shininessStrength, 0.0, 1.0) * shadow;

	return ambient + diffuse + specular;
}

