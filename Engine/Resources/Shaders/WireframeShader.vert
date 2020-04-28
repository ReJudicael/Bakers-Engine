// Attributes
layout(location = 0) in vec3 aPosition;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;

void main()
{
	vec4 pos = uModel * vec4(aPosition, 1.0);
	gl_Position = uProj * uCam * pos;
}