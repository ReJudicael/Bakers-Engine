// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uSecondTexture;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV) + texture(uSecondTexture, vUV);
}