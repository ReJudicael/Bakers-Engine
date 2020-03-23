// Uniforms
uniform sampler2D uColorTexture;
uniform light[10] uLight;
uniform int uLightCount;

struct Material
{
	vec3	diffuseColor;
	vec3	ambientColor;
	vec3	specularColor;
	float	shininess;
	float	shininessStrength;
};

uniform Material mat;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec3 Normal;

void main()
{
    oColor = texture(uColorTexture, vUV);
	vec3 lightContribution;
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], unprojectedPos, Normal, vec3(oColor));
	oColor = vec4(lightContribution, 1.0);
}