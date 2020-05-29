// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;
uniform mat4[10] uLightView;
uniform int uShadowCount;
uniform vec4 cColor;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 unprojectedPos;
out vec4[10] lightSpacePos;
out vec3 camPos;
out vec3 normal;
out vec3 tangent;
out vec4 color;

void main()
{
	vUV = aUV;
	vec4 pos = uModel * vec4(aPosition, 1.0);
	unprojectedPos = vec3(pos);
    camPos = vec3(uCam[3][0], uCam[3][1], uCam[3][2]);
	normal = aNormal;
	tangent = aTangent;
	gl_Position = uProj * uCam * pos;
	
	for (int i = 0; i < uShadowCount; ++i)
		lightSpacePos[i] = uProj * uLightView[i] * pos;

	color = cColor;
}