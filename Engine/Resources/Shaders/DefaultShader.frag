// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uShadowMap;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material mat;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec4 lightSpacePos;
in vec3 camPos;
in vec3 normal;

float shadowComputing(vec4 fragPosFromLight)
{
	vec3 proj = fragPosFromLight.xyz / fragPosFromLight.w;
	proj = proj * 0.5 + 0.5;
	
	float currentDepth = proj.z;
	if (currentDepth > 1.0)
		return 0.0;
	float depth = texture(uShadowMap, proj.xy).r;
	
	// Shadow bias to remove shadow acne
	float bias = 0.0005;
	
	// Compute smooth shadow with PCF
	float shadow = 0;
	vec2 size = 1.0 / textureSize(uShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(uShadowMap, proj.xy + vec2(x, y) * size).r; 
			shadow += currentDepth - bias < pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	
	return shadow;
}

void main()
{
	oColor = texture(uColorTexture, vUV);
	if (uLightCount == 0)
		return;

	vec3 view = normalize(camPos - unprojectedPos);
	vec3 lightContribution;
	float shadow = shadowComputing(lightSpacePos);
	
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], mat, unprojectedPos, normal, view, shadow);
	oColor *= vec4(lightContribution, 1.0);
}