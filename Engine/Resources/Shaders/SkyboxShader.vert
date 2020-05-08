// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 unprojectedPos;
out vec3 camPos;
out vec3 normal;
out vec3 tangent;

void main()
{
	vUV = aUV;
	
	mat4 usedCam = uCam;
	usedCam[3][0] = 0;
	usedCam[3][1] = 0;
	usedCam[3][2] = 0;
	
	vec4 pos = uModel * vec4(aPosition, 1.0);
	gl_Position = uProj * usedCam * pos;
	gl_Position.z = gl_Position.w - 0.000001;
}