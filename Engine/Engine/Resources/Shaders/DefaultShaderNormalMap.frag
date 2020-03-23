// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec3 normal;

// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uNormalMap;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material mat;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(uColorTexture, vUV);
	vec3 normalFromMap = vec3(texture(uNormalMap, vUV));
	
	vec3 lightContribution;
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], mat, unprojectedPos, normal);
	oColor *= vec4(lightContribution, 1.0);
}