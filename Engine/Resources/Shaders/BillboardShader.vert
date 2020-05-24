// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;
uniform vec3 uScale;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 unprojectedPos;
out vec3 camPos;
out vec3 normal;
out vec3 tangent;

void main()
{
	vUV = aUV;
	
	mat4 modelView = uCam * uModel;
	
	// Create new matrix storing everything but rotation
	mat4 usedCam = mat4(1.0);
	usedCam[0][0] = uScale.x;
	usedCam[1][1] = uScale.y;
	usedCam[3][0] = modelView[3][0];
	usedCam[3][1] = modelView[3][1];
	usedCam[3][2] = modelView[3][2];
	usedCam[0][3] = modelView[0][3];
	usedCam[1][3] = modelView[1][3];
	usedCam[2][3] = modelView[2][3];
	
	vec4 pos = vec4(aPosition, 1.0);
	gl_Position = uProj * usedCam * pos;
}