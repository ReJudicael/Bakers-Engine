// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D[10] uShadowMap;
uniform int uShadowCount;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material mat;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec4[10] lightSpacePos;
in vec3 camPos;
in vec3 normal;


void main()
{
	oColor = texture(uColorTexture, vUV);
	if (uLightCount == 0)
		return;

	vec3 view = normalize(camPos - unprojectedPos);
	vec3 lightContribution;
	float shadow = shadowComputing(uShadowCount, lightSpacePos, uShadowMap);
	
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], mat, unprojectedPos, normal, view, shadow);
	oColor *= vec4(lightContribution, 1.0);
}