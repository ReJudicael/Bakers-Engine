// Varyings
in vec2 vUV;
in vec3 unprojectedPos;
in vec3 camPos;
in vec3 normal;
in vec3 tangent;

// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uNormalMap;
uniform light[10] uLight;
uniform int uLightCount;
uniform Material mat;

// Shader outputs
out vec4 oColor;

mat3 GetTBNMatrix(vec3 t, vec3 b, vec3 n)
{
	return mat3(	t.x, b.x, n.x,
			 		t.y, b.y, n.y,
					t.z, b.z, n.z);
}

void main()
{
    oColor = texture(uColorTexture, vUV);
	vec3 normalFromMap = (vec3(texture(uNormalMap, vUV)) * 2.0) - 1.0;
	vec3 bitangent = cross(normal, tangent);
	vec3 newNormal = GetTBNMatrix(tangent, bitangent, normal) * normalFromMap;
	normalize(newNormal);
	vec3 view = normalize(camPos - unprojectedPos);
	
	vec3 lightContribution;
	for (int i = 0; i < uLightCount; i++)
		lightContribution += getLightContribution(uLight[i], mat, unprojectedPos, newNormal, view);
	oColor *= vec4(lightContribution, 1.0);
}