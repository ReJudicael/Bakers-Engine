// Uniforms
uniform sampler2D[10] uShadowMap;
uniform int uShadowCount;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material umat;

// Shader outputs
layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oBrightColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec4[10] lightSpacePos;
in vec3 camPos;
in vec3 normal;
in vec4 color;


void main()
{
	oColor = color;
	if (uLightCount == 0)
		return;

	vec3 view = normalize(camPos - unprojectedPos);
	vec3 lightContribution;
	float shadow = shadowComputing(uShadowCount, lightSpacePos, uShadowMap);
	
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], umat, unprojectedPos, normal, view, shadow);
	oColor *= vec4(lightContribution, 1.0);
	
	if (oColor.r > 1.0 || oColor.g > 1.0 || oColor.b > 1.0)
		oBrightColor = oColor;
	else
		oBrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}