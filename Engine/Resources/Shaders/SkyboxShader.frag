// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
layout (location = 0) out vec4 oColor;
layout (location = 1) out vec4 oBrightColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV);
	
	if (oColor.r > 1.0 || oColor.g > 1.0 || oColor.b > 1.0)
		oBrightColor = oColor;
	else
		oBrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}