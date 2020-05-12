// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in uint aColor;

// Uniforms
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec4 color;
void main()
{
	color = vec4(1, 0, 1, 0.7);
	//color = aColor;
	gl_Position = uProj * uCam * vec4(aPosition, 1);
}