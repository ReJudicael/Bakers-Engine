// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 unprojectedPos;
out vec3 Normal;

void main()
{
	vUV = aUV;
	vec4 pos = uModel * vec4(aPosition, 1.0);
	unprojectedPos = vec3(pos);
    gl_Position = uProj * uCam * pos;
	Normal = aNormal;
}