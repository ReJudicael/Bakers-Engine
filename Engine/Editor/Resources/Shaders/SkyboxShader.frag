// Uniforms
uniform sampler2D uColorTexture;

// Shader outputs
out vec4 oColor;

// Varyings
in vec2 vUV;

void main()
{
	oColor = texture(uColorTexture, vUV);
}