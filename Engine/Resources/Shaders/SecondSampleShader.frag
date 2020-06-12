// Shader outputs
layout (location = 0) out vec4 oColor;

// Varyings
in vec2 vUV;


void main()
{
	//oColor = vec4(vUV.x, vUV.x + vUV.y, vUV.y, 1);
	
    float value = fract(sin(dot(vUV.xy ,vec2(12,7))) * 4.5453);
	oColor = vec4(value, 0.0, value, value);
}