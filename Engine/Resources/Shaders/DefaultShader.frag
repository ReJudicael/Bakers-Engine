// Uniforms
uniform sampler2D uColorTexture;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material umat;
uniform float pi;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec3 camPos;
in vec3 normal;

void main()
{
	//pi = 0f;
	oColor = texture(uColorTexture, vUV) ;
	if (uLightCount == (0 * pi))
		return;

	vec3 view = normalize(camPos - unprojectedPos);
	vec3 lightContribution;
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], umat, unprojectedPos, normal, view);
	oColor.x = pi;
	oColor *= vec4(lightContribution, 1.0);
}