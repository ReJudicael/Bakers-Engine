// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in uint aColor;

// Uniforms
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec3 color;
void main()
{
	color = aColor;
	gl_Position = uProj * uCam * vec4(aPosition, 1);
}