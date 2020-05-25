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
	color = vec4(1, 0, 1, 0.5);
	//color = vec4((aColor & 0x000000FF) >> 0, (aColor & 0x0000FF00) >> 8, (aColor & 0x00FF0000) >> 16, (aColor & 0xFF000000)>> 24) / 255.f;
	gl_Position = uProj * uCam * vec4(aPosition, 1);
}