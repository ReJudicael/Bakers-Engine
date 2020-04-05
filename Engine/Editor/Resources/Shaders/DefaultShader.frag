// Uniforms
uniform sampler2D uColorTexture;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material mat;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec3 camPos;
in vec3 normal;

void main()
{
	oColor = texture(uColorTexture, vUV);
	vec3 view = normalize(camPos - unprojectedPos);
	vec3 lightContribution;
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], mat, unprojectedPos, normal, view);
	oColor *= vec4(lightContribution, 1.0);
}