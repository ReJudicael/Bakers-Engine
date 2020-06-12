// Shader outputs
layout (location = 0) out vec4 oColor;

// Varyings
in vec2 vUV;


void main()
{
	oColor = vec4(vUV.x, vUV.x + vUV.y, vUV.y, 1);
}