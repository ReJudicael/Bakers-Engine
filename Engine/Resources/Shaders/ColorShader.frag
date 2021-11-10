// Shader outputs
layout (location = 0) out vec4 oColor;

// Varyings
in vec2 vUV;
in vec4 color;

void main()
{
	oColor = color;
}